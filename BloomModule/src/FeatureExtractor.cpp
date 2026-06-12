#include "FeatureExtractor.h"

#include <cctype>
#include <cstdint>

py::array_t<float> extract_features(
    const std::vector<std::string>& strings,
    int vector_size
) {
    size_t num_samples = strings.size();

    auto result = py::array_t<float>({num_samples, static_cast<size_t>(vector_size)});
    auto r = result.mutable_unchecked<2>();

    for (size_t i = 0; i < num_samples; i++) {
        for (int j = 0; j < vector_size; j++) {
            r(i, j) = 0.0f;
        }
    }

    for (size_t idx = 0; idx < num_samples; idx++) {
        const std::string& s = strings[idx];
        size_t s_len = s.length();

        if (s_len < 4) {
            continue;
        }

        int digit_count = 0;
        int special_count = 0;
        int transition_count = 0;

        uint32_t h1 = static_cast<uint8_t>(s[0]);
        uint32_t h2 = static_cast<uint8_t>(s[1]);
        uint32_t h3 = static_cast<uint8_t>(s[2]);

        bool last_was_alpha =
            std::isalpha(static_cast<unsigned char>(s[2]));

        for (size_t i = 3; i < s_len; i++) {
            uint32_t val_curr = static_cast<uint8_t>(s[i]);

            bool curr_is_alpha =
                std::isalpha(static_cast<unsigned char>(s[i]));

            if (val_curr >= 48 && val_curr <= 57) {
                digit_count++;
            }
            else if (
                val_curr == 45 ||
                val_curr == 46 ||
                val_curr == 47 ||
                val_curr == 63 ||
                val_curr == 61 ||
                val_curr == 37
            ) {
                special_count++;
            }

            if (curr_is_alpha != last_was_alpha) {
                transition_count++;
            }

            last_was_alpha = curr_is_alpha;

            uint32_t sig3 =
                ((h2 << 10) ^ (h3 << 5) ^ val_curr) % vector_size;

            r(idx, sig3) += 1.0f;

            uint32_t sig4 =
                ((h1 << 15) ^
                 (h2 << 10) ^
                 (h3 << 5) ^
                 val_curr) % vector_size;

            r(idx, sig4) += 0.5f;

            h1 = h2;
            h2 = h3;
            h3 = val_curr;
        }

        float inv_len = 1.0f / static_cast<float>(s_len);

        for (int j = 0; j < vector_size; j++) {
            r(idx, j) *= inv_len;
        }

        r(idx, 0) = static_cast<float>(s_len) * 0.01f;
        r(idx, 1) = digit_count * inv_len;
        r(idx, 2) = special_count * inv_len;
        r(idx, 3) = transition_count * inv_len;
    }

    return result;
}