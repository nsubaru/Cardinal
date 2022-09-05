# Cardinal

Cardinal - is a flexible high-performance framework based on C++20 modules architecture. Because of skipping some abstraction layers and working directly with Windows kernel, this allows you to make more optimized programs.

The framework includes:

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
  * **Security** - the API for working with NT security descriptor;
  * **Serialization** - the API for fast binary serialization and deserialization the data using the compile-time type information;
  * **Text** - a set of classes for simplify work with some text data;
  * **Tokenizer** - a class for splitting string by set of delimiters that can be recursive;
  * **Threading** - the API for working with threads, thread pool and async/await. It also contains sub-modules:
      * **SRWLock** - fast read/write lock object;
      * **DataFlowControler** - fast producer/consumer lock for queues;
      
... and more!
