# DynamicArray&lt;T&gt;

`DynamicArray<T>`는 요소를 연속된 메모리 공간에 저장하는 가변 크기 배열 컨테이너입니다.  
빠른 임의 접근을 제공하고, 임의 위치에서의 삽입 및 삭제를 지원합니다. 요소 추가와 제거에 따라 메모리 용량을 자동으로 관리합니다.

---

## 시간 복잡도 (Time Complexity)

- 임의 접근: O(1)
- 끝에 삽입/삭제: 분할 상환 O(1) (Amortized O(1))
- 중간 삽입/삭제: O(n)
- 탐색: O(n)

---

## 인터페이스 (Interface)

### 상수 (Constants)

- `constexpr std::size_t INDEX_NONE` (static)  
  요소를 찾지 못했거나 유효하지 않은 인덱스를 나타내는 특수 값입니다.

### 생성자 / 소멸자 (Constructors / Destructor)

- `DynamicArray()`  
  빈 배열을 생성합니다.

- `DynamicArray(std::size_t count)`  
  `count`개의 요소가 기본값으로 초기화된 배열을 생성합니다.

- `DynamicArray(std::size_t count, const T& value)`  
  `value`의 복사본 `count`개로 구성된 배열을 생성합니다.

- `DynamicArray(std::initializer_list<T> init)`  
  초기화 리스트로 배열을 생성합니다.

- `DynamicArray(const DynamicArray& other)`  
  복사 생성자. `other`의 모든 요소를 깊은 복사합니다.

- `DynamicArray(DynamicArray&& other)`  
  이동 생성자. `other`의 자원 소유권을 이전합니다.

- `~DynamicArray()`  
  모든 요소를 제거하고 할당된 메모리를 해제합니다.

### 대입 연산자 (Assignment Operators)

- `DynamicArray& operator=(const DynamicArray& other)`  
  복사 대입 연산자.

- `DynamicArray& operator=(DynamicArray&& other)`  
  이동 대입 연산자.

- `DynamicArray& operator=(std::initializer_list<T> ilist)`  
  초기화 리스트의 값으로 배열을 설정합니다.

### 요소 접근 (Element Access)

- `T& operator[](std::size_t index)`  
  `const T& operator[](std::size_t index)`  
  `index` 위치의 요소에 접근합니다.  
  범위를 벗어나면 `std::out_of_range` 예외를 발생시킵니다.

- `T* Data()`  
  `const T* Data()`  
  내부 데이터 배열의 원시 포인터를 반환합니다.

### 용량 관리 (Capacity)

- `bool IsEmpty()`  
  배열이 비어 있으면 `true`를 반환합니다.

- `std::size_t Size()`  
  현재 저장된 요소의 개수를 반환합니다.

- `std::size_t Capacity()`  
  현재 할당된 메모리 용량을 반환합니다.

- `void Reserve(std::size_t newCapacity)`  
  최소 `newCapacity`만큼의 저장 공간을 미리 확보합니다.

- `void Shrink()`  
  용량을 현재 요소 개수에 맞춰 축소합니다.  
  배열이 비어 있는 경우 메모리를 완전히 해제합니다.

### 수정자 (Modifiers)

- `void Add(const T& value)`  
  `void Add(T&& value)`  
  배열의 끝에 요소를 추가합니다.

- `void AddRange(std::initializer_list<T> ilist)`  
  배열의 끝에 초기화 리스트의 모든 요소를 추가합니다.

- `T& Emplace(Args&&... args)`  
  배열의 끝에 요소를 직접 생성하여 추가합니다.

- `T& EmplaceAt(std::size_t index, Args&&... args)`  
  지정된 위치에 요소를 직접 생성하여 삽입합니다.

- `void Insert(std::size_t index, const T& value)`  
  `void Insert(std::size_t index, T&& value)`  
  지정된 위치에 요소를 삽입합니다.

- `void InsertRange(std::size_t index, std::initializer_list<T> ilist)`  
  지정된 위치에 여러 요소를 한 번에 삽입합니다.

- `bool Remove(const T& value)`  
  배열에서 처음 발견된 `value`를 제거하고, 성공적으로 제거한 경우 `true`를 반환합니다.

- `void RemoveAt(std::size_t index)`  
  지정된 위치의 요소를 제거합니다.

- `void RemoveRange(std::size_t index, std::size_t count)`  
  `index`부터 `count`개의 요소를 제거합니다.

- `std::size_t RemoveAll(Predicate pred)`  
  조건식 `pred`를 만족하는 모든 요소를 제거하고, 제거된 개수를 반환합니다.

- `void Clear()`  
  모든 요소를 제거합니다. 할당된 용량은 유지됩니다.

- `void Resize(std::size_t newSize)`  
  `void Resize(std::size_t newSize, const T& value)`  
  배열의 크기를 변경합니다.  
  크기가 증가하는 경우 새 요소를 `value`로 초기화하고, 감소하는 경우 초과된 요소를 제거합니다.

- `void Swap(DynamicArray& other)`  
  두 배열의 내용을 서로 교환합니다.

### 탐색 (Lookup)

- `bool Contains(const T& value)`  
  배열에 `value`가 존재하는지 확인합니다.

- `bool ContainsIf(Predicate pred)`  
  조건식 `pred`를 만족하는 요소가 있는지 확인합니다.

- `std::size_t Find(const T& value)`  
  `value`의 첫 번째 인덱스를 반환합니다.

- `std::size_t FindIf(Predicate pred)`  
  조건식 `pred`를 만족하는 첫 번째 요소의 인덱스를 반환합니다.

- `std::size_t FindLast(const T& value)`  
  `value`의 마지막 인덱스를 반환합니다.

- `std::size_t FindLastIf(Predicate pred)`  
  조건식 `pred`를 만족하는 마지막 요소의 인덱스를 반환합니다.

### 비멤버 함수 (Non-member Functions)

- `bool operator==(const DynamicArray& lhs, const DynamicArray& rhs)`  
  두 배열의 크기와 모든 요소가 같은지 비교합니다.

- `auto operator<=>(const DynamicArray& lhs, const DynamicArray& rhs)`  
  두 배열을 사전순으로 비교합니다.

---

## 사용 예시 (Usage Example)

```cpp
#include <iostream>
#include "DynamicArray.h"

int main()
{
    DynamicArray<int> arr = {1, 2, 3};               // [1, 2, 3]

    arr.Add(4);                                      // [1, 2, 3, 4]
    arr.Emplace(5);                                  // [1, 2, 3, 4, 5]

    arr.Insert(2, 99);                               // [1, 2, 99, 3, 4, 5]
    arr.InsertRange(0, {100, 101});                  // [100, 101, 1, 2, 99, 3, 4, 5]

    arr.RemoveRange(0, 2);                           // [1, 2, 99, 3, 4, 5]
    arr.Remove(99);                                  // [1, 2, 3, 4, 5]

    std::size_t index = arr.Find(3);
    if (index != DynamicArray<int>::INDEX_NONE)
    {
        std::cout << "발견된 인덱스: " << index << "\n";
    }

    arr.Reserve(100);
    arr.Shrink();

    try
    {
        int value = arr[10]; // 범위를 벗어난 접근
    }
    catch (const std::out_of_range& e)
    {
        std::cerr << "에러: " << e.what() << "\n";
    }

    return 0;
}
