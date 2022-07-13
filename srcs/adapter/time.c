#include "convert.h"

bool
timelog_enqueue (timelog_t ** list, long time)
{
	timelog_t * current;
	timelog_t * node;

	node = (timelog_t *) malloc (sizeof (timelog_t));
	if (!node)
	{
		return false;
	}
	node->time = time;
	node->next = NULL;

	if (*list == NULL)
	{
		*list = node;
		return true;
	}
	current = *list;
	while (current->next)
	{
		current = current->next;
	}
	current->next = node;
	return true;
}
