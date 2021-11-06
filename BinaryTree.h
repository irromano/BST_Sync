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
struct p *removeValHelper(int v, struct p *somewhere, struct p *parent);
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
 * Removes an int node p from a BT of p nodes. The
 * root is returned.
 */
struct p *removeVal(int v, struct p *somewhere)
{
    if (v == somewhere->v) /* Value is root */
    {
        struct p *del = somewhere;
        if (somewhere->left != NULL && somewhere->right != NULL)
        {
            struct p *temp = somewhere->left;
            somewhere = somewhere->right;
            addNode(temp, somewhere);
            killNode(del);
            return somewhere;
        }

        else
        {
            somewhere = (somewhere->left == NULL) ? somewhere->right : somewhere->left;
            killNode(del);
            return somewhere;
        }
    }

    else if (v <= somewhere->v)
    {
        removeValHelper(v, somewhere->left, somewhere);
        return somewhere;
    }

    else /* v > somewhere->v */
    {
        removeValHelper(v, somewhere->right, somewhere);
        return somewhere;
    }
}

/*
 * Helper functinon of removeVal, which recursively searches
 * for the node to delete if the original root was not the
 * value v.
 */
struct p *removeValHelper(int v, struct p *somewhere, struct p *parent)
{
    if (v < somewhere->v)
    {
        if (somewhere->left != NULL)
        {
            return removeValHelper(v, somewhere->left, somewhere);
        }
    }
    else if (v > somewhere->v)
    {
        if (somewhere->right != NULL)
        {
            return removeValHelper(v, somewhere->right, somewhere);
        }
    }
    else if (v == somewhere->v)
    {
        if (parent->left == somewhere)
        {
            if (somewhere->left != NULL && somewhere->right != NULL)
            {
                struct p *temp = somewhere->left;
                parent->left = somewhere->right;
                addNode(temp, parent->left);
                killNode(somewhere);
                return parent;
            }

            else if (somewhere->right == NULL)
            {
                parent->left = somewhere->left;
                killNode(somewhere);
                return parent;
            }
            else /* somewhere->right == NULL */
            {
                parent->left = somewhere->right;
                killNode(somewhere);
                return parent;
            }
        }
        else if (parent->right == somewhere)
        {
            if (somewhere->left != NULL && somewhere->right != NULL)
            {
                struct p *temp = somewhere->left;
                parent->right = somewhere->right;
                addNode(temp, parent->right);
                killNode(somewhere);
                return parent;
            }
            else if (somewhere->right == NULL)
            {
                parent->right = somewhere->left;
                killNode(somewhere);
                return parent;
            }
            else /* somewhere->right == NULL */
            {
                parent->right = somewhere->right;
                killNode(somewhere);
                return parent;
            }
        }
    }
    return NULL;
}

/*
 * Add a p node from a node to a BST.
 */
int addNode(struct p *node, struct p *root)
{

    struct p *temp = root;
    while (temp != NULL)
    {
        if (node->v <= temp->v)
        {
            if (temp->left == NULL)
            {
                temp->left = node;
                return node->v;
            }
            temp = temp->left;
        }
        else
        {
            if (temp->right == NULL)
            {
                temp->right = node;
                return node->v;
            }
            temp = temp->right;
        }
    }
    /* Should not reach here */
    return node->v;
}

void killNode(struct p *node)
{
    node->left = 0x0;
    node->right = 0x0;
    node->v = 0;
    free(node);
    return;
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
    {
        valid *= root->left->v <= root->v;
        if (!valid)
        {
            printf("Integrity failed at root-v == %d\r\n", root->v);
            return 0;
        }
    }
    if (root->right != NULL)
    {
        valid *= root->v < root->right->v;
        if (!valid)
        {
            printf("Integrity failed at root-v == %d\r\n", root->v);
            return 0;
        }
    }

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