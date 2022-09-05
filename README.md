# Cardinal

Cardinal - is a flexible high-performance framework based on C++20 modules architecture. Skipping some abstraction layers and working directly with the Windows kernel allows you to make more optimized programs.

Cardinal includes:

  * **Collections** - a basic set of collections;
  * **Crypt** - a set of the encryption functions;
  * **CTTI** - compile-time type information data;
  * **Dependency injection**;
  * **Exceptions** - modified Windows x64 exception ABI and set of basic exceptions;
  * **Functionality** - a collection of types to simplify functional style programing;
  * **Logger** - a base logging module;
  * **Math** - a set of math functions, which supports the compile-time calculations;
  * **Memory** - low-level memory work API;
  * **RTTI** - runtime type information data;
  * **Security** - API for working with NT security descriptor;
  * **Serialization** - API for fast binary serialization and deserialization of data using the compile-time type information;
  * **Text** - a set of classes for simplifying work with some text data;
  * **Tokenizer** - a class for splitting a string by a set of delimiters that can be recursive;
  * **Threading** - API for working with threads, thread pool, async/await. It also contains sub-modules:
      * **SRWLock** - fast read/write lock object;
      * **DataFlowControler** - fast producer/consumer lock for queues;

... and more!
