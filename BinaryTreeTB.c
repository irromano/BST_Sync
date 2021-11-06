#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "BinaryTree.h"

int main()
{
    int arr1[] = {10, 5, 2, 12, 6, 7};

    int len1 = sizeof(arr1) / sizeof(arr1[0]);
    printf("The size of arr1 is %d\r\n", len1);

    struct p *root = (struct p *)malloc(sizeof(struct p));
    root->v = arr1[0];
    for (int i = 1; i < len1; i++)
    {
        add(arr1[i], root);
    }

    printNodes(root);
    printf("The total node count is %d\r\n", size(root));
    root = removeVal(10, root);
    printNodes(root);
    printf("The total node count is %d\r\n", size(root));
    printf("The validity of the BT is %d\r\n", checkIntegrity(root));

    for (int i = 0; i < 10; i++)
    {
        add(10, root);
        root = removeVal(10, root);
        printNodes(root);
        printf("The total node count is %d\r\n", size(root));
        printf("The validity of the BT is %d\r\n", checkIntegrity(root));
    }

    // root->left->v = 20;
    // printf("The validity of the BT is %d\r\n", checkIntegrity(root));
}