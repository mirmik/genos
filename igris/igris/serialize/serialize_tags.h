#ifndef IGRIS_SERIALIZE_SERIALIZE_TAGS_H__
#define IGRIS_SERIALIZE_SERIALIZE_TAGS_H__

namespace igris
{
    template <class Container> class serialize_list_tag
    {
    public:
        std::remove_const_t<Container> &container;
        serialize_list_tag(Container &container)
            : container(const_cast<std::remove_const_t<Container> &>(container))
        {
        }
        size_t size() { return container.size(); }
    };

    template <class Container> class serialize_dict_tag
    {
    public:
        Container &container;
        serialize_dict_tag(Container &container) : container(container) {}
        size_t size() { return container.size(); }
    };
}

#endif