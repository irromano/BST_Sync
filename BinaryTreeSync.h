#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>

#ifndef BINARYTREESYNC_H
#define BINARYTREESYNC_H

struct p
{
    int v;
    struct p *left;
    struct p *right;
    pthread_mutex_t lock;
};

struct p *add(int v, struct p *somewhere);
struct p *removeVal(int v, struct p *somewhere);
struct p *removeValHelper(int v, struct p *somewhere, struct p *parent);
struct p *addNode(struct p *node, struct p *root);
int size(struct p *root);
int checkIntegrity(struct p *root);
void printNodes(struct p *root);
struct p *killNode(struct p *node);

/*
 * Adds an int node p to a BT of p nodes. This malloc's
 * the node onto the heap and must be manually freed.
 */
struct p *add(int v, struct p *somewhere)
{
    pthread_mutex_lock(&(somewhere->lock));

    struct p *node = (struct p *)malloc(sizeof(struct p));
    pthread_mutex_init(&(node->lock), NULL);
    pthread_mutex_lock(&(node->lock));
    node->left = NULL;
    node->right = NULL;
    node->v = v;

    struct p *parent = somewhere;
    while (parent != NULL)
    {
        if (v <= parent->v)
        {
            if (parent->left == NULL || parent->left->v == 0)
            {
                parent->left = node;
                pthread_mutex_unlock(&(node->lock));
                pthread_mutex_unlock(&(parent->lock));
                return node;
            }
            pthread_mutex_lock(&(parent->left->lock));
            struct p *nextParent = parent->left;
            pthread_mutex_unlock(&(parent->lock));
            parent = nextParent;
        }
        else /* v > parent->v */
        {
            if (parent->right == NULL || parent->right->v == 0)
            {
                parent->right = node;
                pthread_mutex_unlock(&(node->lock));
                pthread_mutex_unlock(&(parent->lock));
                return node;
            }
            pthread_mutex_lock(&(parent->right->lock));
            struct p *nextParent = parent->right;
            pthread_mutex_unlock(&(parent->lock));
            parent = nextParent;
        }
    }
    pthread_mutex_unlock(&(node->lock));
    killNode(node);
    pthread_mutex_unlock(&(somewhere->lock));
    return node;
}

/*
 * Removes an int node p from a BT of p nodes. The
 * root is returned.
 */
struct p *removeVal(int v, struct p *somewhere)
{
    if (somewhere == NULL)
        return somewhere;

    pthread_mutex_lock(&(somewhere->lock));

    if (v == somewhere->v) /* Value is root */
    {
        struct p *del = somewhere;
        if (somewhere->left != NULL && somewhere->right != NULL && somewhere->left->v != 0 && somewhere->right->v != 0)
        {
            pthread_mutex_lock(&(somewhere->left->lock));
            struct p *temp = somewhere->left;
            pthread_mutex_lock(&(somewhere->right->lock));
            somewhere = somewhere->right;
            struct p *temp2 = addNode(temp, somewhere);
            del = killNode(del);
            pthread_mutex_unlock(&(temp2->lock));
            return somewhere;
        }

        else if (somewhere->left != NULL && somewhere->left->v != 0)
        {
            pthread_mutex_lock(&(somewhere->left->lock));
            somewhere = somewhere->left;
            del = killNode(del);
            pthread_mutex_unlock(&(somewhere->lock));
            return somewhere;
        }
        else if (somewhere->right != NULL && somewhere->right->v != 0)
        {
            pthread_mutex_lock(&(somewhere->right->lock));
            somewhere = somewhere->right;
            del = killNode(del);
            pthread_mutex_unlock(&(somewhere->lock));
            return somewhere;
        }
        else
        {
            somewhere = NULL;
            del = killNode(del);
            return somewhere;
        }
    }

    else if (v <= somewhere->v && somewhere->left != NULL && somewhere->left->v != 0)
    {
        pthread_mutex_lock(&(somewhere->left->lock));
        struct p *deletedParent = removeValHelper(v, somewhere->left, somewhere);
        pthread_mutex_unlock(&(deletedParent->lock));
        return somewhere;
    }

    else if (v > somewhere->v && somewhere->right != NULL && somewhere->right->v != 0)
    {
        pthread_mutex_lock(&(somewhere->right->lock));
        struct p *deletedParent = removeValHelper(v, somewhere->right, somewhere);
        pthread_mutex_unlock(&(deletedParent->lock));
        return somewhere;
    }

    /*else  v is not in this BST */
    pthread_mutex_unlock(&(somewhere->lock));
    return somewhere;
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
        if (somewhere->left != NULL && somewhere->left->v != 0)
        {
            pthread_mutex_lock(&(somewhere->left->lock));
            pthread_mutex_unlock(&(parent->lock));
            return removeValHelper(v, somewhere->left, somewhere);
        }
    }
    else if (v > somewhere->v)
    {
        if (somewhere->right != NULL && somewhere->right->v != 0)
        {
            pthread_mutex_lock(&(somewhere->right->lock));
            pthread_mutex_unlock(&(parent->lock));
            return removeValHelper(v, somewhere->right, somewhere);
        }
    }
    if (v == somewhere->v)
    {
        if (parent->left == somewhere)
        {
            if (somewhere->left != NULL && somewhere->right != NULL && somewhere->left->v != 0 && somewhere->right->v != 0)
            {
                pthread_mutex_lock(&(somewhere->left->lock));
                struct p *delLeft = somewhere->left;
                pthread_mutex_lock(&(somewhere->right->lock));
                parent->left = somewhere->right;
                somewhere = killNode(somewhere);
                pthread_mutex_unlock(&(parent->lock));
                parent = addNode(delLeft, parent->left);
                return parent;
            }

            else if (somewhere->left != NULL && somewhere->left->v != 0)
            {
                pthread_mutex_lock(&(somewhere->left->lock));
                parent->left = somewhere->left;
                somewhere = killNode(somewhere);
                pthread_mutex_unlock(&(parent->left->lock));
                return parent;
            }
            else if (somewhere->right != NULL && somewhere->right->v != 0)
            {
                pthread_mutex_lock(&(somewhere->right->lock));
                parent->left = somewhere->right;
                somewhere = killNode(somewhere);
                pthread_mutex_unlock(&(parent->left->lock));
                return parent;
            }
            else
            {
                parent->left = NULL;
                somewhere = killNode(somewhere);
                return parent;
            }
        }
        if (parent->right == somewhere)
        {
            if (somewhere->left != NULL && somewhere->right != NULL && somewhere->left->v != 0 && somewhere->right->v != 0)
            {
                pthread_mutex_lock(&(somewhere->left->lock));
                struct p *delLeft = somewhere->left;
                pthread_mutex_lock(&(somewhere->right->lock));
                parent->right = somewhere->right;
                somewhere = killNode(somewhere);
                pthread_mutex_unlock(&(parent->lock));
                parent = addNode(delLeft, parent->right);
                return parent;
            }
            else if (somewhere->left != NULL && somewhere->left->v != 0)
            {
                pthread_mutex_lock(&(somewhere->left->lock));
                parent->right = somewhere->left;
                somewhere = killNode(somewhere);
                pthread_mutex_unlock(&(parent->right->lock));
                return parent;
            }
            else if (somewhere->right != NULL && somewhere->right->v != 0)
            {
                pthread_mutex_lock(&(somewhere->right->lock));
                parent->right = somewhere->right;
                somewhere = killNode(somewhere);
                pthread_mutex_unlock(&(parent->right->lock));
                return parent;
            }
            else
            {
                parent->right = NULL;
                somewhere = killNode(somewhere);
                return parent;
            }
        }
    }
    pthread_mutex_unlock(&(somewhere->lock));
    return parent;
}

/*
 * Add a p node from a node to a BST.
 */
struct p *addNode(struct p *node, struct p *root)
{
    if (node == NULL)
        return root;

    struct p *temp = root;
    while (temp != NULL)
    {
        if (node->v <= temp->v)
        {
            if (temp->left == NULL || temp->left->v == 0)
            {
                temp->left = node;
                pthread_mutex_unlock(&(node->lock));
                return temp;
            }
            struct p *temp2 = temp;
            temp = temp->left;
            pthread_mutex_unlock(&(temp2->lock));
            pthread_mutex_lock(&(temp->lock));
        }
        else
        {
            if (temp->right == NULL || temp->right->v == 0)
            {
                temp->right = node;
                pthread_mutex_unlock(&(node->lock));
                return temp;
            }
            struct p *temp2 = temp;
            temp = temp->right;
            pthread_mutex_unlock(&(temp2->lock));
            pthread_mutex_lock(&(temp->lock));
        }
    }
    /* Should not reach here */
    printf("Error in adding node\n");
    return node;
}

struct p *killNode(struct p *node)
{
    node->left = NULL;
    node->right = NULL;
    node->v = 0;
    free(node); /* Freeing node afterwards randomly assigned variables to that node */
    pthread_mutex_unlock(&(node->lock));
    pthread_mutex_destroy(&(node->lock));
    // free(node);
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