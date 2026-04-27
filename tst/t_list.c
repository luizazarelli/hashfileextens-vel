#include "unity.h"
#include "list.h"

void setUp(void) {}
void tearDown(void) {}

void test_create_and_size_zero(void) {
    Lista l = createList();
    TEST_ASSERT_NOT_NULL(l);
    TEST_ASSERT_EQUAL_INT(0, getSize(l));
    TEST_ASSERT_NULL(getHead(l));
    freeList(l);
}

void test_insert_single(void) {
    Lista l = createList();
    int val = 42;
    void* node = insertAtTail(l, &val);
    TEST_ASSERT_NOT_NULL(node);
    TEST_ASSERT_EQUAL_INT(1, getSize(l));
    TEST_ASSERT_EQUAL_PTR(&val, getData(getHead(l)));
    freeList(l);
}

void test_insert_multiple_order(void) {
    Lista l = createList();
    int a = 1, b = 2, c = 3;
    insertAtTail(l, &a);
    insertAtTail(l, &b);
    insertAtTail(l, &c);
    TEST_ASSERT_EQUAL_INT(3, getSize(l));

    void* n1 = getHead(l);
    void* n2 = getNext(n1);
    void* n3 = getNext(n2);
    TEST_ASSERT_EQUAL_PTR(&a, getData(n1));
    TEST_ASSERT_EQUAL_PTR(&b, getData(n2));
    TEST_ASSERT_EQUAL_PTR(&c, getData(n3));
    TEST_ASSERT_NULL(getNext(n3));
    freeList(l);
}

void test_remove_head(void) {
    Lista l = createList();
    int a = 1, b = 2;
    void* na = insertAtTail(l, &a);
    insertAtTail(l, &b);
    removeNode(l, na);
    TEST_ASSERT_EQUAL_INT(1, getSize(l));
    TEST_ASSERT_EQUAL_PTR(&b, getData(getHead(l)));
    freeList(l);
}

void test_remove_middle(void) {
    Lista l = createList();
    int a = 1, b = 2, c = 3;
    insertAtTail(l, &a);
    void* nb = insertAtTail(l, &b);
    insertAtTail(l, &c);
    removeNode(l, nb);
    TEST_ASSERT_EQUAL_INT(2, getSize(l));
    void* n1 = getHead(l);
    void* n2 = getNext(n1);
    TEST_ASSERT_EQUAL_PTR(&a, getData(n1));
    TEST_ASSERT_EQUAL_PTR(&c, getData(n2));
    TEST_ASSERT_NULL(getNext(n2));
    freeList(l);
}

void test_remove_tail(void) {
    Lista l = createList();
    int a = 1, b = 2;
    insertAtTail(l, &a);
    void* nb = insertAtTail(l, &b);
    removeNode(l, nb);
    TEST_ASSERT_EQUAL_INT(1, getSize(l));
    TEST_ASSERT_NULL(getNext(getHead(l)));
    freeList(l);
}

int main(void) {
    UNITY_BEGIN();
    RUN_TEST(test_create_and_size_zero);
    RUN_TEST(test_insert_single);
    RUN_TEST(test_insert_multiple_order);
    RUN_TEST(test_remove_head);
    RUN_TEST(test_remove_middle);
    RUN_TEST(test_remove_tail);
    return UNITY_END();
}
