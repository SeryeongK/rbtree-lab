#include "rbtree.h"
#include <stdlib.h>

rbtree *new_rbtree(void)
{
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // 🚨 TODO: initialize struct if needed

  // nil 포인터 초기화(NULL이 아님) => nil 포인터가 SENTINEL 노드를 가리키도록
  // SENTINEL: 리프 노드들이 가리키는 NULL 포인터 대신 / 루트 노드의 부모 노드는 SENTINEL 노드
  p->nil = (node_t *)malloc(sizeof(node_t));
  p->nil->color = RBTREE_BLACK;
  p->nil->parent = p->nil->left = p->nil->right = p->nil;
  // root 포인터 초기화
  p->root = p->nil;
  return p;
}

// RB 트리 구조체가 차지했던 메모리 반환
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  free(t);
}

// 왼쪽으로 회전하는 함수
rbtree *left_rotate_rbtree(rbtree *t, node_t *x)
{
  node_t *yptr = x->right;
  x->right = yptr->left;
  if (yptr->left != t->nil)
  {
    yptr->left->parent = x;
  };
  yptr->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = yptr;
  }
  else if (x == x->parent->left)
  {
    x->parent->left = yptr;
  }
  else
  {
    x->parent->right = yptr;
  }
  yptr->left = x;
  x->parent = yptr;
  return t;
};

// 오른쪽으로 회전하는 함수
rbtree *right_rotate_rbtree(rbtree *t, node_t *x)
{
  node_t *yptr = x->left;
  x->left = yptr->right;
  if (yptr->right != t->nil)
  {
    yptr->right->parent = x;
  };
  yptr->parent = x->parent;
  if (x->parent == t->nil)
  {
    t->root = yptr;
  }
  else if (x == x->parent->right)
  {
    x->parent->right = yptr;
  }
  else
  {
    x->parent->left = yptr;
  }
  yptr->right = x;
  x->parent = yptr;
  return t;
}

// 삽입 후 흐트러진 트리 정렬
rbtree *rbtree_insert_fixup(rbtree *t, node_t *z)
{
  while (z->parent->color == RBTREE_RED)
  {
    if (z->parent == z->parent->parent->left)
    {
      node_t *yptr = z->parent->parent->right;
      if (yptr->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        yptr->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->right)
        {
          z = z->parent;
          left_rotate_rbtree(t, z);
        };
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        right_rotate_rbtree(t, z->parent->parent);
      }
    }
    else
    {
      node_t *yptr = z->parent->parent->left;
      if (yptr->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        yptr->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        if (z == z->parent->left)
        {
          z = z->parent;
          right_rotate_rbtree(t, z);
        };
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate_rbtree(t, z->parent->parent);
      };
    }
  }
  t->root->color = RBTREE_BLACK;
  return t;
}

node_t *rbtree_insert(rbtree *t, const key_t key)
{
  // TODO: implement insert
  node_t *xptr = t->root;
  node_t *yptr = t->nil;
  node_t *newNodeptr = (node_t *)malloc(sizeof(node_t));
  newNodeptr->key = key;

  while (xptr != t->nil) // 첫 노드는 xptr이 t->nil
  {
    yptr = xptr;
    if (newNodeptr->key < xptr->key)
    {
      xptr = xptr->left;
    }
    else
    {
      xptr = xptr->right;
    }
  };
  newNodeptr->parent = yptr;
  if (yptr == t->nil) // 현재 newNodeptr도 parent를 가리킴
  {
    t->root = newNodeptr;
  }
  else if (newNodeptr->key < yptr->key)
  {
    yptr->left = newNodeptr;
  }
  else
  {
    yptr->right = newNodeptr;
  }
  newNodeptr->left = t->nil;
  newNodeptr->right = t->nil;
  newNodeptr->color = RBTREE_RED;
  // 삽입 후 rotate
  rbtree_insert_fixup(t, newNodeptr);
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key)
{
  // TODO: implement find
  node_t *findptr = (node_t *)malloc(sizeof(node_t));
  // 트리에 노드가 없으면
  if (t->root == t->nil)
  {
    return NULL;
  }
  // 트리에 노드가 있으면, findptr의 초기값을 root 노드로
  findptr = t->root;
  while (findptr != t->nil)
  {
    if (findptr->key == key)
    {
      break;
    }
    else if (findptr->key > key)
    {
      findptr = findptr->left;
    }
    else
    {
      findptr = findptr->right;
    }
  }
  // 트리에 해당 노드가 없으면
  if (findptr->key != key)
  {
    return NULL;
  }
  else
  {
    return findptr;
  }
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
  {
    return NULL;
  }
  node_t *findptr = (node_t *)malloc(sizeof(node_t));
  node_t *parentptr = (node_t *)malloc(sizeof(node_t));
  findptr = t->root;
  while (findptr != t->nil)
  {
    parentptr = findptr;
    findptr = findptr->left;
  }
  return parentptr;
}

node_t *rbtree_max(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
  {
    return NULL;
  }
  node_t *findptr = (node_t *)malloc(sizeof(node_t));
  node_t *parentptr = (node_t *)malloc(sizeof(node_t));
  findptr = t->root;
  while (findptr != t->nil)
  {
    parentptr = findptr;
    findptr = findptr->right;
  }
  return parentptr;
}

int rbtree_erase(rbtree *t, node_t *p)
{
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  return 0;
}
