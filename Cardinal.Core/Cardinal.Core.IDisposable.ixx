module;
#include "Cardinal.Core.Definitions.h"

export module Cardinal.Core:IDisposable;

import :Types;
import :TypeTraits;
import :Concepts;

export namespace Cardinal::Core
{
    /// <summary>
    /// Class resources disposing interface with reference count
    /// </summary>
    VirtualClass IDisposable
    {
    public:
        [[msvc::noop_dtor]] virtual ~IDisposable() {};

    protected:
        /// <summary>
        /// A method for resource disposing, call automatically by destructor in classes with reference count
        /// </summary>
        virtual void Dispose() = 0;
    };
}

