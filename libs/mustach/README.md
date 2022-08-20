# Introduction to Mustach 1.2

`mustach` is a C implementation of the [mustache](http://mustache.github.io "main site for mustache")
template specification.

The main site for `mustach` is on [gitlab](https://gitlab.com/jobol/mustach).

The simpliest way to use mustach is to copy the files **mustach.h** and **mustach.c**
directly into your project and use it.

If you are using one of the JSON libraries listed below, you can get extended feature
by also including **mustach-wrap.h**, **mustach-wrap.c**, **mustach-XXX.h** and
**mustach-XXX.c** in your project (see below for **XXX**)

- [json-c](https://github.com/json-c/json-c): use **XXX** = **json-c**
- [jansson](http://www.digip.org/jansson/): use **XXX** = **jansson**
- [cJSON](https://github.com/DaveGamble/cJSON): use **XXX** = **cjson**

Alternatively, make and meson files are provided for building `mustach` and
`libmustach.so` shared library.

Since version 1.0, the makefile allows to compile and install different
flavours. See below for details.

## Distributions offering mustach package

### Alpine Linux

```sh
apk add mustach
apk add mustach-lib
apk add mustach-dev
```

### NetBSD

```sh
cd devel/mustach
make
```

See http://pkgsrc.se/devel/mustach

## Known projects using Mustach

This [wiki page](https://gitlab.com/jobol/mustach/-/wikis/projects-using-mustach)
lists the known project that are using mustach and that kindly told it.

Don't hesitate to tell us if you are interested to be listed there.

## Using Mustach from sources

The file **mustach.h** is the main documentation. Look at it.

The current source files are:

- **mustach.c** core implementation of mustache in C
- **mustach.h** header file for core definitions
- **mustach-wrap.c** generic wrapper of mustach for easier integration
- **mustach-wrap.h** header file for using mustach-wrap
- **mustach-json-c.c** tiny json wrapper of mustach using [json-c](https://github.com/json-c/json-c)
- **mustach-json-c.h** header file for using the tiny json-c wrapper
- **mustach-cjson.c** tiny json wrapper of mustach using [cJSON](https://github.com/DaveGamble/cJSON)
- **mustach-cjson.h** header file for using the tiny cJSON wrapper
- **mustach-jansson.c** tiny json wrapper of mustach using [jansson](https://www.digip.org/jansson/)
- **mustach-jansson.h** header file for using the tiny jansson wrapper
- **mustach-tool.c** simple tool for applying template files to one JSON file

The file **mustach-json-c.c** is the historical example of use of **mustach** and
**mustach-wrap** core and it is also a practical implementation that can be used.
It uses the library json-c. (NOTE for Mac OS: available through homebrew).

Since version 1.0, the project also provide integration of other JSON libraries:
**cJSON** and **jansson**.

*If you integrate a new library with* **mustach**, *your contribution will be
welcome here*.

The tool **mustach** is build using `make`,  its usage is:

    mustach json template [template]...

It then outputs the result of applying the templates files to the JSON file.

### Portability

Some system does not provide *open_memstream*. In that case, tell your
prefered compiler to declare the preprocessor symbol **NO_OPEN_MEMSTREAM**.
Example:

	CFLAGS=-DNO_OPEN_MEMSTREAM make

### Integration

The files **mustach.h** and **mustach-wrap.h** are the main documentation. Look at it.

The file **mustach-json-c.c** provides a good example of integration.

If you intend to use basic HTML/XML escaping and standard C FILE, the callbacks
of the interface **mustach_itf** that you have to implement are:
`enter`, `next`, `leave`, `get`.

If you intend to use specific escaping and/or specific output, the callbacks
of the interface **mustach_itf** that you have to implement are:
`enter`, `next`, `leave`, `get` and `emit`.

### Compilation Using Make

Building and installing can be done using make.

Example:

    $ make tool=cjson libs=none PREFIX=/usr/local DESTDIR=/ install
    $ make tool=jsonc libs=single PREFIX=/ DESTDIR=$HOME/.local install

The makefile knows following switches (\*: default):

     Switch name  | Values  | Description
    --------------+---------+-----------------------------------------------
     jsonc        | (unset) | Auto detection of json-c
                  | no      | Don't compile for json-c
                  | yes     | Compile for json-c that must exist
    --------------+---------+-----------------------------------------------
     cjson        | (unset) | Auto detection of cJSON
                  | no      | Don't compile for cJSON
                  | yes     | Compile for cJSON that must exist
    --------------+---------+-----------------------------------------------
     jansson      | (unset) | Auto detection of jansson
                  | no      | Don't compile for jansson
                  | yes     | Compile for jansson that must exist
    --------------+---------+-----------------------------------------------
     tool         | (unset) | Auto detection
                  | cjson   | Use cjson library
                  | jsonc   | Use jsonc library
                  | jansson | Use jansson library
                  | none    | Don't compile the tool
    --------------+---------+----------------------------------------------
     libs         | (unset) | Like 'all'
                  | all     | Like 'single' AND 'split'
                  | single  | Only libmustach.so
                  | split   | All the possible libmustach-XXX.so ...
                  | none    | No library is produced

The libraries that can be produced are:

     Library name       | Content
    --------------------+--------------------------------------------------------
     libmustach-core    | mustach.c mustach-wrap.c
     libmustach-cjson   | mustach.c mustach-wrap.c mustach-cjson.c
     libmustach-jsonc   | mustach.c mustach-wrap.c mustach-json-c.c
     libmustach-jansson | mustach.c mustach-wrap.c mustach-jansson.c
     libmustach         | mustach.c mustach-wrap.c mustach-{cjson,json-c,jansson}.c

There is no dependencies of a library to an other. This is intended and doesn't
hurt today because the code is small.

## Extensions

The current implementation provides extensions to specifications of **mustache**.
This extensions can be activated or deactivated using flags.

Here is the summary.

     Flag name                     | Description
    -------------------------------+------------------------------------------------
     Mustach_With_Colon            | Explicit tag substition with colon
     Mustach_With_EmptyTag         | Empty Tag Allowed
    -------------------------------+------------------------------------------------
     Mustach_With_Equal            | Value Testing Equality
     Mustach_With_Compare          | Value Comparing
     Mustach_With_JsonPointer      | Interpret JSON Pointers
     Mustach_With_ObjectIter       | Iteration On Objects
     Mustach_With_EscFirstCmp      | Escape First Compare
     Mustach_With_ErrorUndefined   | Error when a requested tag is undefined
    -------------------------------+------------------------------------------------
     Mustach_With_AllExtensions    | Activate all known extensions
     Mustach_With_NoExtensions     | Disable any extension

For the details, see below.

### Explicit Tag Substitution With Colon (Mustach_With_Colon)

In somecases the name of the key used for substition begins with a
character reserved for mustach: one of `#`, `^`, `/`, `&`, `{`, `>` and `=`.

This extension introduces the special character `:` to explicitly
tell mustach to just substitute the value. So `:` becomes a new special
character.

This is a core extension implemented in file **mustach.c**.

### Empty Tag Allowed (Mustach_With_EmptyTag)

When an empty tag is found, instead of automatically raising the error
MUSTACH\_ERROR\_EMPTY\_TAG pass it.

This is a core extension implemented in file **mustach.c**.

### Value Testing Equality (Mustach_With_Equal)

This extension allows you to test the value of the selected key.
It allows to write `key=value` (matching test) or `key=!value`
(not matching test) in any query.

This is a wrap extension implemented in file **mustach-wrap.c**.

### Value Comparing (Mustach_With_Compare)

These extension extends the extension for testing equality to also
compare values if greater or lesser.
Its allows to write `key>value` (greater), `key>=value` (greater or equal),
`key<value` (lesser) and `key<=value` (lesser or equal).

It the comparator sign appears in the first column it is ignored
as if it was escaped.

This is a wrap extension implemented in file **mustach-wrap.c**.

### Interpret JSON Pointers (Mustach_With_JsonPointer)

This extension allows to use JSON pointers as defined in IETF RFC 6901.
If active, any key starting with "/" is a JSON pointer.
This implies to use the colon to introduce JSON keys.

A special escaping is used for `=`, `<`, `>` signs when
values comparisons are enabled: `~=` gives `=` in the key.

This is a wrap extension implemented in file **mustach-wrap.c**.

### Iteration On Objects (Mustach_With_ObjectIter)

With this extension, using the pattern `{{#X.*}}...{{/X.*}}`
allows to iterate on fields of `X`.

Example:

- `{{s.*}} {{*}}:{{.}}{{/s.*}}` applied on `{"s":{"a":1,"b":true}}` produces ` a:1 b:true`

Here the single star `{{*}}` is replaced by the iterated key
and the single dot `{{.}}` is replaced by its value.

This is a wrap extension implemented in file **mustach-wrap.c**.

### Error when a requested tag is undefined (Mustach_With_ErrorUndefined)

Report the error MUSTACH_ERROR_UNDEFINED_TAG when a requested tag
is not defined.

This is a wrap extension implemented in file **mustach-wrap.c**.

### Access To Current Value

*this was an extension but is now always enforced*

The value of the current field can be accessed using single dot.

Examples:

- `{{#key}}{{.}}{{/key}}` applied to `{"key":3.14}` produces `3.14`
- `{{#array}} {{.}}{{/array}}` applied to `{"array":[1,2]}` produces ` 1 2`.

This is a wrap extension implemented in file **mustach-wrap.c**.

### Partial Data First

*this was an extension but is now always enforced*

The default resolution for partial pattern like `{{> name}}`
is to search for `name` in the current json context and
as a file named `name` or if not found `name.mustache`.

By default, the order of the search is (1) as a file,
and if not found, (2) in the current json context.

When this option is set, the order is reverted and content
of partial is search (1) in the current json context,
and if not found, (2) as a file.

That option is useful to keep the compatibility with
versions of *mustach* anteriors to 1.2.0.

This is a wrap extension implemented in file **mustach-wrap.c**.

### Escape First Compare

This extension automatically escapes comparisons appears as
first characters.

This is a wrap extension implemented in file **mustach-wrap.c**.

## Difference with version 0.99 and previous

### Extensions

The extensions can no more be removed at compile time, use
flags to select your required extension on need.

### Name of functions

Names of functions were improved. Old names remain but are obsolete
and legacy. Their removal in far future versions is possible.

The table below summarize the changes.

     legacy name      | name since version 1.0.0
    ------------------+-----------------------
     fmustach         | mustach_file
     fdmustach        | mustach_fd
     mustach          | mustach_mem
     fmustach_json_c  | mustach_json_c_file
     fdmustach_json_c | mustach_json_c_fd
     mustach_json_c   | mustach_json_c_mem
     mustach_json_c   | mustach_json_c_write

