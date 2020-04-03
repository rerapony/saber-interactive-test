#include <cassert>
#include "binary.hpp"
#include "serialization.hpp"
#include "normals.h"

void test_serialize_deserialize() {
    List list;

    list.push_back("This");
    list.push_back("is");

    auto* node = new ListNode("a");
    node->rand = list.front();
    list.push_node(node);

    node = new ListNode("simple");
    node->rand = list.back();
    list.push_node(node);

    list.push_back("test\n");

    FILE* file = fopen("../src/file.txt", "wb");
    assert(file != nullptr);

    list.Serialize(file);

    fclose(file);

    file = fopen("../src/file.txt", "rb");
    assert(file != nullptr);

    List other;
    other.Deserialize(file);

    fclose(file);

    assert(list == other);
}

int main() {
    binary(INT32_MIN);

    test_serialize_deserialize();

    std::cout << std::endl;
    std::cout << "All tests passed" << std::endl;
    return 0;
}