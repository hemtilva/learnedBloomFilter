class BloomFilter:
    def __init__(self, element_count: int, fpr: float) -> None: ...
    def add(self, data: str) -> None: ...
    def contains(self, data: str) -> bool: ...
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

class Evaluator:
    def generate_random_strings(
        self,
        count: int,
        maxLenOfStrings: int,
        path: str
    ) -> list[str]: ...

    def run_single_experiment(
        self,
        elementCount: int,
        fpr: float,
        pathReal: str,
        pathFake: str
    ) -> Result: ...