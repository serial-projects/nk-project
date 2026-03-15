# NK Project

NK Project (or New Key Project) is a set of types and implementaitons to aid C 
programming. Things like containers and controllers.

## Features
1. `NK_Vector`, `NK_Map`, `NK_Set` for "advanced containers";
2. `NK_SubmergedString` for pooled strings (avoids string repeation);
3. `NK_DynamicString` faster and "ephemeral" strings;
4. `NK_Validator` an logger merged with a validator;
5. `NK_Allocator` with support of statistics;
6. `NK_Reader` and `NK_Writer` for IO operations and generic interface;
7. Written in ANSI C.

And many more.

## Licensing

`NK` is part of the `Ruka Raposa Engine` project, which is all under the GPL-3
license.

## Using on your project

`NK` uses CMake, you can just:

```cmake
add_subdirectory(NK)
```

And use the target "NK", which is linked statically automatically, NK is not to
be built dynamically!

> Options include:
1. `NK_ENABLE_ASAN` if you want to use ASan inside `NK`;

2. `NK_ENABLE_TESTS` if you want to (for some reason) build the tests.

## Pattern

`NK` functions exist on the `NK_` namespace, they all come with the:
```c

/** 
 * @brief This creates an new `NK_Object` on the heap.
 * 
 * @warning Does not imply on `NK_ObjectConstruct`, NK demands explicit
 * initialization!
 */
NK_Object* NK_ObjectNew();

/** @brief This frees the `NK_Object` created by `NK_ObjectNew` function. */
void NK_ObjectFree(NK_Object* object);

/** @brief This constructs the `NK_Object` internal buffers, etc. */
void NK_ObjectConstruct(NK_Object* object, ...);

/**
 * @brief This destructs the `NK_Object` internal buffers, etc.
 * 
 * @warning Does not imply on `NK_ObjectFree`!
 */
void NK_ObjectDestruct(NK_Object* object);
```

This is the pattern, if you are using `plugins`, you can simply type `NK_` and
see the functions yourself.

## Objective

We don't aim to be the `fastest` (neither the slowest), `NK` is meant to be
simple to integrate to any project, because it was made to `Ruka Raposa Engine`
and while you can use it to other projects, the objetive is to simply make
some functionalities to C that `someone` might need, like `NK_DynamicString`(s)
or `NK_Map`(s) for example.

You may contribute by reporting bugs on this github page. 

Made With S2 by Ruka Raposa Team, 2025 - 2026!