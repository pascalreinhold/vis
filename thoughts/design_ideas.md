# Design Ideas / Rules

## General
1. Functions should never have hidden side effects
2. Use **const** and **nodiscard** where appropriate 
3. If a member functions depends on the state of the object **assert** the correctness of that state
4. **m_** prefix for member variables in Non-POD types
5. Use the Logger instead of **std::cout** or **std::cerr** or throwing an exception for fatal errors

## Vulkan Object Wrappers
1. Automatic Cleanup (RAII)
