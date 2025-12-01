#include <igris/series/block.h>
#include <igris/series/iterator.h>
#include <igris/series/series.h>

void *igris::series_iterator::pointer()
{
    return block()->get(num);
}

const void *igris::series_iterator::pointer() const
{
    assert(block());
    return block()->get(num);
}

igris::series_iterator &igris::series_iterator::operator++()
{
    num++;
    if (num == block()->fini)
    {
        block_lnk = block_lnk->next;
        num = 0;
    }

    return *this;
}

igris::series_iterator igris::series_iterator::operator++(int)
{
    igris::series_iterator iter = *this;
    ++*this;
    return iter;
}

igris::series_iterator &igris::series_iterator::operator--()
{
    num--;
    if (num == block()->strt - 1)
    {
        block_lnk = block_lnk->prev;
        num = 0;
    }
    return *this;
}

igris::series_iterator igris::series_iterator::operator--(int)
{
    igris::series_iterator iter = *this;
    --*this;
    return iter;
}

igris::series_block *igris::series_iterator::block()
{
    return dlist_entry(block_lnk, series_block, lnk);
}

const igris::series_block *igris::series_iterator::block() const
{
    return dlist_entry(block_lnk, series_block, lnk);
}

igris::series_iterator::series_iterator(dlist_head *block_lnk, size_t num)
    : block_lnk(block_lnk), num(num)
{
}
