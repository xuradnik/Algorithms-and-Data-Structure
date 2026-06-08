#ifndef LIBDS_UTILS_HPP
#define LIBDS_UTILS_HPP

#include <stdexcept>
#include <utility>


namespace ds::utils {


/**
 * @brief Destroys all objects in the specified range by calling their destructors.
 * @tparam BlockType The type of the objects.
 * @param first Pointer to the first object to be destroyed.
 * @param last Pointer to the position after the last object.
 */
template<typename BlockType>
void destroyObjects(BlockType *first, BlockType *const last) {
    while (first != last) {
        first->~BlockType();
        ++first;
    }
}

/**
 * @brief Copies objects from a source range to a destination address using placement new.
 * @tparam BlockType The type of the objects.
 * @param dst Pointer to the destination memory.
 * @param src Pointer to the first source object.
 * @param srcEnd Pointer to the position after the last source object.
 */
template<typename BlockType>
void copyObjects(BlockType *dst, BlockType *src, BlockType *const srcEnd) {
    while (src != srcEnd) {
        new (dst) BlockType(*src);
        ++src;
        ++dst;
    }
}

/**
 * @brief Moves objects from a source range to a destination address and destroys the originals.
 * @tparam BlockType The type of the objects.
 * @param dst Pointer to the destination memory.
 * @param src Pointer to the first source object.
 * @param srcEnd Pointer to the position after the last source object.
 */
template<typename BlockType>
void rellocateObjects(BlockType *dst, BlockType *src, BlockType *const srcEnd) {
    while (src != srcEnd) {
        new (dst) BlockType(std::move(*src));
        src->~BlockType();
        ++src;
        ++dst;
    }
}

/**
 * @brief Relocates objects in reverse order to handle overlapping memory regions.
 * @tparam BlockType The type of the objects.
 * @param dst Pointer to the destination memory (trailing end).
 * @param src Pointer to the source object to start from.
 * @param srcEnd Pointer to the stopping boundary.
 */
template<typename BlockType>
void rellocateObjectsBackwards(BlockType *dst, BlockType *src, BlockType *const srcEnd) {
    while (src != srcEnd) {
        new (dst) BlockType(std::move(*src));
        src->~BlockType();
        --src;
        --dst;
    }
}


/**
 * @brief Thrown when a function that is not available for a type is called.
 */
class unavailable_function_call : public std::runtime_error {
public:
    unavailable_function_call() : std::runtime_error(
        "Unavailable function call.") {
    }
};


/**
 * @brief Thrown when parameter type is different than expected.
 */
class type_missmatch : public std::invalid_argument {
public:
    type_missmatch () : std::invalid_argument("Type missmatch.") {
    }
};


} // namespace ds::utils


#endif
