#ifndef ASYNC_H
#define ASYNC_H

#if defined(_MSC_VER) || defined(WIN64) || defined(_WIN64) || defined(__WIN64__) || defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
#  define Q_DECL_EXPORT __declspec(dllexport)
#  define Q_DECL_IMPORT __declspec(dllimport)
#else
#  define Q_DECL_EXPORT     __attribute__((visibility("default")))
#  define Q_DECL_IMPORT     __attribute__((visibility("default")))
#endif

#if defined(ASYNC_LIBRARY)
#  define ASYNC_EXPORT Q_DECL_EXPORT
#else
#  define ASYNC_EXPORT Q_DECL_IMPORT
#endif

#include <cstddef>

namespace async {

using handle_t = void *;

ASYNC_EXPORT handle_t connect(std::size_t bulk);
ASYNC_EXPORT void receive(handle_t handle, const char *data, std::size_t size);
ASYNC_EXPORT void disconnect(handle_t handle);

}

#endif // ASYNC_H
