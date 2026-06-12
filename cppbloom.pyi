from typing import List
import numpy as np
import numpy.typing as npt


def extract_features(
    strings: List[str],
    vector_size: int
) -> npt.NDArray[np.float32]: ...


class BloomFilter:
    def __init__(self, element_count: int, fpr: float) -> None: ...
    def add(self, data: str) -> None: ...
    def contains(self, data: str) -> bool: ...
    def __contains__(self, data: str) -> bool: ...
    def memory_cost(self) -> int: ...


class Result:
    insertCount: int
    negativeQueryCount: int
    truePositive: int
    falsePositive: int
    trueNegative: int
    falseNegative: int
    timeTakenInNs: int
    memoryNeeded: int

    def __repr__(self) -> str: ...


class Evaluator:
    def __init__(self) -> None: ...

    def generate_random_strings(
        self,
        count: int,
        maxLenOfStrings: int,
        path: str
    ) -> List[str]: ...

    def run_single_experiment(
        self,
        elementCount: int,
        fpr: float,
        pathReal: str,
        pathFake: str
    ) -> Result: ...