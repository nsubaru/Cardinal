#pragma once
#include "Cardinal.RTTI.Base.Base.h"

// Magic number
#pragma section(".type$a", read, write) 

// Header
#pragma section(".type$b", read, write) 

// Pointer subsection which contain actual addresses to MetaData structs
#pragma section(".type$fa", read, write) 
#pragma section(".type$fb", read, write) 
#pragma section(".type$fz", read, write) 

// Data subsection which contain MetaData structs
#pragma section(".type$va", read, write) 
#pragma section(".type$vb", read, write) 
#pragma section(".type$vz", read, write) 

#pragma section(".type$z", read, write) 