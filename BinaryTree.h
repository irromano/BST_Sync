#include <stdlib.h>
#include <stdio.h>

#ifndef BINARYTREE_H
#define BINARYTREE_H

struct p
{
    int v;
    struct p *left;
    struct p *right;
};

struct p *add(int v, struct p *somewhere);
struct p *removeVal(int v, struct p *somewhere, struct p *parent);
int size(struct p *root);
int checkIntegrity(struct p *root);
void printNodes(struct p *root);

/*
 * Adds an int node p to a BT of p nodes. This malloc's
 * the node onto the heap and must be manually freed.
 */
struct p *add(int v, struct p *somewhere)
{
    struct p *node = (struct p *)malloc(sizeof(struct p));
    node->v = v;

    struct p *temp = somewhere;
    while (temp != NULL)
    {
        if (v <= temp->v)
        {
            if (temp->left == NULL)
            {
                temp->left = node;
                return node;
            }
            temp = temp->left;
        }
        else
        {
            if (temp->right == NULL)
            {
                temp->right = node;
                return node;
            }
            temp = temp->right;
        }
    }

    return node;
}

/*
 * Removes an int node p from a BT of p nodes. This node
 * is returned and must be freed by the user.
 */
struct p *removeVal(int v, struct p *somewhere, struct p *parent)
{
    if (v < somewhere->v)
    {
        if (somewhere->left != NULL)
        {
            return removeVal(v, somewhere->left, somewhere);
        }
    }
    else if (v > somewhere->v)
    {
        if (somewhere->right != NULL)
        {
            return removeVal(v, somewhere->right, somewhere);
        }
    }
    else if (v == somewhere->v)
    {
        if (somewhere->left == NULL)
        {
            if (parent == NULL)
            {
                parent = somewhere;
                somewhere = somewhere->right;
                return parent;
            }
            else if (parent->left == somewhere)
            {
                parent->left = somewhere->right;
                return somewhere;
            }
            else /* parent->right == somewhere */
            {
                parent->right = somewhere->right;
                return somewhere;
            }
        }
        else if (somewhere->right == NULL)
        {
            if (parent == NULL)
            {
                parent = somewhere;
                somewhere = somewhere->left;
                return parent;
            }
            else if (parent->left == somewhere)
            {
                parent->left = somewhere->left;
                return somewhere;
            }
            else /* parent->right == somewhere */
            {
                parent->right = somewhere->left;
                return somewhere;
            }
        }
        else /* both somewhere->left and somewhere->right != NULL */
        {
            struct p *temp = somewhere->right;
            while (temp->left != NULL)
                temp = temp->left;
            somewhere->v = temp->v;
            return removeVal(temp->v, somewhere->right, somewhere);
        }
    }
    return NULL;
}

/*
 * Counts all nodes and returns count.
 */
int size(struct p *root)
{
    if (root == NULL)
        return 0;

    int total = 0;

    total += size(root->left);
    total++;
    total += size(root->right);
    return total;
}

/**
 * Checks Integrity of BT. Returns 1 if order
 * is properly formed, otherwise returns 0.
 */
int checkIntegrity(struct p *root)
{
    if (root == NULL)
        return 1;

    int valid = 1;

    if (root->left != NULL)
        valid *= root->left->v <= root->v;
    if (root->right != NULL)
        valid *= root->v < root->right->v;

    valid *= checkIntegrity(root->left);
    valid *= checkIntegrity(root->right);
    return valid;
}

/*
 * Prints all nodes connected to the root
 * node given.
 */
void printNodes(struct p *root)
{
    if (root == NULL)
        return;

    printNodes(root->left);
    printf("Node: %d\r\n", root->v);
    printNodes(root->right);
}

#endif