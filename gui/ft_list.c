#include "ft_list.h"
#include <string.h>
#include <unistd.h>

FTList *ft_list_first(FTList *list)
{
    while (list && list->prev) list = list->prev;

    return list;
}

FTList *ft_list_last(FTList *list)
{
    while (list && list->next) list = list->next;

    return list;
}

FTList *ft_list_append(FTList *list, void *data)
{
    FTList *node = malloc(sizeof(FTList));

    memset(node, 0, sizeof(node));

    list = ft_list_last(list);

    if (list)
    {
        list->next = node;
        node->prev = list;
    }

    node->data = data;

    return ft_list_first(node);
}

FTList *ft_list_prepend(FTList *list, void *data)
{
    FTList *node = malloc(sizeof(FTList));

    memset(node, 0, sizeof(node));

    list = ft_list_first(list);

    if (list)
    {
        list->prev = node;
        node->next = list;
    }

    node->data = data;

    return node;
}

FTList *ft_list_insert(FTList *list, void *data, int position)
{
    FTList *node = NULL;

    list = ft_list_nth(list, position);

    if (list)
    {
        node = malloc(sizeof(FTList));

        node->next = list;
        node->prev = list->prev;
        node->data = data;

        list->prev = node;
    }

    return ft_list_first(node);
}

FTList *ft_list_delete(FTList *list, void *data)
{
    FTList *iter;

    iter = ft_list_first(list);

    for (; iter; iter = iter->next)
    {
        if (iter->data == data)
        {
            iter->prev->next = iter->next;
            iter->next->prev = iter->prev;
            
            if (list == iter)
                list = list->next;

            free(iter);

            break;
        }
    }

    return ft_list_first(list);
}

FTList *ft_list_nth(FTList *list, int nth)
{
    int i = 0;

    list = ft_list_first(list);

    while (list && list->next && i++ < nth) list = list->next;

    return list;
}

FTList *ft_list_find(FTList *list, void *data)
{
    FTList *iter = ft_list_first(list);

    for (; iter; iter = iter->next)
    {
        if (iter->data == data) 
            break;
    }

    return iter;
}

void *ft_list_nth_data(FTList *list, int nth)
{
    list = ft_list_nth(list, nth);

    return list ? list->data : NULL;
}

void ft_list_free(FTList *list, int free_nodes)
{
    FTList *temp;

    list = ft_list_first(list);

    while (list)
    {
        if (free_nodes)
            free(list->data);

        temp = list;
        list = list->next;

        free(temp);
    }
}

