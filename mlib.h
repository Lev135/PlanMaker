#ifndef MLIB_H
#define MLIB_H

#define M_ENUM_BEGIN(name)\
    namespace namespace_##name {\
        enum name {

#define M_ENUM_END\
        };\
    }

#endif // MLIB_H
