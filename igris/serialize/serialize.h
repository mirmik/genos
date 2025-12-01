#ifndef IGRIS_SERIALIZE_SERIALIZE_H
#define IGRIS_SERIALIZE_SERIALIZE_H

/**
@file
*/

#include <igris/serialize/archive.h>

/*
    NOTE: Архивы содержат шаблонную реализацию методов load или dump,
    работающую через метод reflect, реализованный в классе
    сериализуемого объекта. В свою очередь по оператору & в
    методе reflect вызывается serialize/deserialize.

    serialize/deserialize имплементированны через load/dump методы архивов
    для базовых типов, типов, содержащих reflect, для стандартных небазовых
    имплементированы seialize_helper-ы. Также, при необходимости можно
    отдельно прописать в классе методы serialize/deserialize.
*/

#endif
