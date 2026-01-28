# LinkedList&lt;T&gt;

`LinkedList<T>`는 노드들이 포인터로 연결되어 비연속적인 메모리 공간에 요소를 저장하는 이중 연결 리스트 컨테이너입니다.  
각 노드는 이전과 다음 노드에 대한 포인터를 가지며, 전방과 후방 양방향 순회를 지원합니다.  
노드 포인터를 알고 있는 경우 어느 위치에서든 O(1) 시간 복잡도로 삽입 및 삭제가 가능하며, 임의 접근이 필요한 경우에는 순차 탐색이 필요합니다.

---

## 시간 복잡도 (Time Complexity)

- 양 끝 삽입/삭제: O(1)
- 임의 위치 삽입/삭제: O(1)
- 탐색: O(n)

---

## 인터페이스 (Interface)

### 클래스 (Class)

- `Node`  
  리스트의 각 요소를 나타내는 노드 클래스입니다. 데이터와 양방향 연결 정보를 담고 있습니다.

  - `T& Value()`  
    `const T& Value()`  
    저장된 데이터의 참조를 반환합니다.

  - `Node* Next()`  
    `const Node* Next()`  
    다음 노드의 포인터를 반환합니다. 마지막 노드일 경우 `nullptr`를 반환합니다.

  - `Node* Prev()`  
    `const Node* Prev()`  
    이전 노드의 포인터를 반환합니다. 첫 번째 노드일 경우 `nullptr`를 반환합니다.

### 생성자 / 소멸자 (Constructors / Destructor)

- `LinkedList()`  
   빈 리스트를 생성합니다.

- `LinkedList(std::size_t count)`  
  `count`개의 요소가 기본값으로 초기화된 리스트를 생성합니다.

- `LinkedList(std::size_t count, const T& value)`  
  `value`의 복사본 `count`개로 구성된 리스트를 생성합니다.

- `LinkedList(std::initializer_list<T> init)`  
  초기화 리스트로 리스트를 생성합니다.

- `LinkedList(const LinkedList& other)`  
  복사 생성자. `other`의 모든 요소를 깊은 복사합니다.

- `LinkedList(LinkedList&& other)`  
  이동 생성자. `other`의 자원 소유권을 이전합니다.

- `~LinkedList()`  
  모든 노드를 메모리에서 해제합니다.

### 대입 연산자 (Assignment Operators)

- `LinkedList& operator=(const LinkedList& other)`  
  복사 대입 연산자.

- `LinkedList& operator=(LinkedList&& other)`  
  이동 대입 연산자.

- `LinkedList& operator=(std::initializer_list<T> ilist)`  
  초기화 리스트의 값으로 리스트를 설정합니다.

### 요소 접근 (Element Access)

- `Node* Head()`  
  `const Node* Head()`  
  리스트의 첫 번째 노드를 반환합니다. 리스트가 비어 있으면 `nullptr`를 반환합니다.

- `Node* Tail()`  
  `const Node* Tail()`  
  리스트의 마지막 노드를 반환합니다. 리스트가 비어 있으면 `nullptr`를 반환합니다.

### 용량 관리 (Capacity)

- `bool IsEmpty()`  
  리스트 비어 있으면 `true`를 반환합니다.

- `std::size_t Size()`  
  현재 저장된 요소의 개수를 반환합니다.

### 수정자 (Modifiers)

- `void AddHead(const T& value)`  
  `void AddHead(T&& value)`  
  리스트의 맨 앞에 요소를 추가합니다.

- `void AddTail(const T& value)`  
  `void AddTail(T&& value)`  
  리스트의 맨 뒤에 요소를 추가합니다.

- `Node* Emplace(Node* before, Args&&... args)`  
  `before` 노드 앞에 요소를 직접 생성하여 삽입합니다.  
  `before`가 `nullptr`인 경우 리스트의 맨 뒤에 추가합니다.

- `Node* EmplaceHead(Args&&... args)`  
  리스트의 맨 앞에 요소를 직접 생성하여 삽입합니다.

- `Node* EmplaceTail(Args&&... args)`  
  리스트의 맨 뒤에 요소를 직접 생성하여 삽입합니다.

- `void Insert(Node* before, const T& value)`  
  `void Insert(Node* before, T&& value)`  
  `before` 노드 앞에 요소를 삽입합니다.

- `bool Remove(const T& value)`  
  값이 일치하는 첫 번째 노드를 제거합니다. 성공적으로 제거한 경우 `true`를 반환합니다.

- `void Remove(Node* node)`  
  지정된 노드를 리스트에서 제거합니다. `node`가 `nullptr`이면 아무런 동작도 하지 않습니다.

- `void RemoveHead()`  
  첫 번째 노드를 제거합니다.

- `void RemoveTail()`  
  마지막 노드를 제거합니다.

- `void Clear()`  
  모든 노드를 제거하여 리스트를 비웁니다.

- `void Resize(std::size_t count)`  
  `void Resize(std::size_t count, const T& value)`  
  리스트의 크기를 변경합니다.  
  크기가 증가하는 경우 새 요소를 `value`로 초기화하고, 감소하는 경우 초과된 요소를 제거합니다.

- `void Swap(LinkedList& other)`  
  두 배열의 내용을 서로 교환합니다.

### 탐색 (Lookup)

- `bool Contains(const T& value)`  
  리스트에 `value`가 존재하는지 확인합니다.

- `Node* Find(const T& value)`  
  `const Node* Find(const T& value)`  
  값이 일치하는 첫 번째 노드를 반환합니다. 없으면 `nullptr`를 반환합니다.

- `Node* FindLast(const T& value)`  
  `const Node* FindLast(const T& value)`  
  값이 일치하는 마지막 노드를 반환합니다. 없으면 `nullptr`를 반환합니다.

### 비멤버 함수 (Non-member Functions)

- `bool operator==(const LinkedList& lhs, const LinkedList& rhs)`  
  두 리스트의 크기와 모든 요소가 같은지 비교합니다.

- `auto operator<=>(const LinkedList& lhs, const LinkedList& rhs)`  
  두 리스트를 사전순으로 비교합니다.

---

## 사용 예시 (Usage Example)

```cpp
#include <iostream>
#include "LinkedList.h"

int main()
{
    LinkedList<int> list = {10, 20, 30};           // [10, 20, 30]
    
    list.AddHead(5);                               // [5, 10, 20, 30]
    list.AddTail(40);                              // [5, 10, 20, 30, 40]
    
    auto* node = list.Find(20);
    if (node)
    {
        list.Insert(node, 15);                     // [5, 10, 15, 20, 30, 40]
        auto* newNode = list.Emplace(node, 18);    // [5, 10, 15, 18, 20, 30, 40]
        
        std::cout << "삽입된 값: " << newNode->Value() << "\n";
    }
    
    list.Remove(30);                               // [5, 10, 15, 18, 20, 40]
    list.RemoveHead();                             // [10, 15, 18, 20, 40]
    list.RemoveTail();                             // [10, 15, 18, 20]
    
    std::cout << "정방향 순회: ";
    for (auto* cur = list.Head(); cur; cur = cur->Next())
    {
        std::cout << cur->Value() << " ";
    }
    std::cout << "\n";

    std::cout << "역방향 순회: ";
    for (auto* cur = list.Tail(); cur; cur = cur->Prev())
    {
        std::cout << cur->Value() << " ";
    }
    std::cout << "\n";

    return 0;
}
