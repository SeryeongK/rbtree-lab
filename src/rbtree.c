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

void delete_rbtree_node(rbtree *t, node_t *n)
{
  if (n == t->nil)
  {
    return;
  }
  delete_rbtree_node(t, n->left);
  delete_rbtree_node(t, n->right);
  free(n);
}

// RB 트리 구조체가 차지했던 메모리 반환
void delete_rbtree(rbtree *t)
{
  // TODO: reclaim the tree nodes's memory
  if (t->root != t->nil)
  {
    delete_rbtree_node(t, t->root);
  }
  free(t->nil);
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
  node_t *findptr;
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
      return findptr;
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
  return NULL;
}

node_t *rbtree_min(const rbtree *t)
{
  // TODO: implement find
  if (t->root == t->nil)
  {
    return NULL;
  }
  node_t *parentptr;
  node_t *findptr = t->root;
  while (findptr != t->nil)
  {
    parentptr = findptr;
    findptr = findptr->left;
  }
  return parentptr;
}

// 가능하면 위의 함수로 병합
node_t *rbtree_sub_min(const rbtree *t, node_t *r)
{
  // TODO: implement find
  if (r == t->nil)
  {
    return r;
  }
  node_t *parentptr;
  node_t *findptr = r;
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
  node_t *parentptr;
  node_t *findptr = t->root;
  while (findptr != t->nil)
  {
    parentptr = findptr;
    findptr = findptr->right;
  }
  return parentptr;
}

void rbtree_transplant(rbtree *t, node_t *from, node_t *to)
{
  if (from->parent == t->nil)
  {
    t->root = to;
  }
  else if (from == from->parent->left)
  {
    from->parent->left = to;
  }
  else
  {
    from->parent->right = to;
  }
  to->parent = from->parent;
  return;
}

// 삭제 후 흐트러진 트리 조정
void rbtree_erase_fixup(rbtree *t, node_t *x)
{
  node_t *siblingptr;
  while (x != t->root && x->color == RBTREE_BLACK)
  {
    if (x == x->parent->left)
    {
      siblingptr = x->parent->right;
      if (siblingptr->color == RBTREE_RED)
      {
        siblingptr->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        left_rotate_rbtree(t, x->parent);
        siblingptr = x->parent->right;
      }
      if (siblingptr->left->color == RBTREE_BLACK && siblingptr->right->color == RBTREE_BLACK)
      {
        siblingptr->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (siblingptr->right->color == RBTREE_BLACK)
        {
          siblingptr->left->color = RBTREE_BLACK;
          siblingptr->color = RBTREE_RED;
          right_rotate_rbtree(t, siblingptr);
          siblingptr = x->parent->right;
        }
        siblingptr->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        siblingptr->right->color = RBTREE_BLACK;
        left_rotate_rbtree(t, x->parent);
        x = t->root;
      }
    }
    else
    {
      siblingptr = x->parent->left;
      if (siblingptr->color == RBTREE_RED)
      {
        siblingptr->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate_rbtree(t, x->parent);
        siblingptr = x->parent->left;
      }
      if (siblingptr->right->color == RBTREE_BLACK && siblingptr->left->color == RBTREE_BLACK)
      {
        siblingptr->color = RBTREE_RED;
        x = x->parent;
      }
      else
      {
        if (siblingptr->left->color == RBTREE_BLACK)
        {
          siblingptr->right->color = RBTREE_BLACK;
          siblingptr->color = RBTREE_RED;
          left_rotate_rbtree(t, siblingptr);
          siblingptr = x->parent->left;
        }
        siblingptr->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        siblingptr->left->color = RBTREE_BLACK;
        right_rotate_rbtree(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;
}

// RB 트리 내부의 ptr로 지정된 노드를 삭제하고 메모리 반환
int rbtree_erase(rbtree *t, node_t *p) // 🚨 void로 반환해도 됨 / 왜 리턴 타입이 int?
{
  // TODO: implement erase
  // node_t *yptr = (node_t *)malloc(sizeof(node_t));
  node_t *yptr = p;
  color_t y_original_color = yptr->color;
  // node_t *xptr = (node_t *)malloc(sizeof(node_t));
  node_t *xptr = p;
  if (p->left == t->nil)
  {
    xptr = p->right;
    rbtree_transplant(t, p, p->right);
  }
  else if (p->right == t->nil)
  {
    xptr = p->left;
    rbtree_transplant(t, p, p->left);
  }
  else
  {
    yptr = rbtree_sub_min(t, p->right);
    y_original_color = yptr->color;
    xptr = yptr->right;
    if (yptr != p->right)
    {
      rbtree_transplant(t, yptr, yptr->right);
      yptr->right = p->right;
      yptr->right->parent = yptr;
    }
    else
    {
      xptr->parent = yptr;
    }
    rbtree_transplant(t, p, yptr);
    yptr->left = p->left;
    yptr->left->parent = yptr;
    yptr->color = p->color;
  }
  if (y_original_color == RBTREE_BLACK)
  {
    rbtree_erase_fixup(t, xptr);
  }
  free(p);
  return 0;
}

void inorder(node_t *root, node_t *nil, key_t *arr, int *index)
{
  if (root == nil)
  {
    return;
  }
  inorder(root->left, nil, arr, index);  // 현재 루트 기준 왼쪽 서브트리
  arr[(*index)++] = root->key;           // 현재 루트의 값을 배열에 저장
  inorder(root->right, nil, arr, index); // 현재 루트 기준 오른쪽 서브트리
}

// 중위순회
int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n)
{
  // TODO: implement to_array
  int *index = (int *)calloc(1, sizeof(int)); // 배열의 인덱스를 가리킬 포인터
  inorder(t->root, t->nil, arr, index);       // 중위순회
  free(index);                                // 인덱스 해제
  return 0;
}
