# Inasehash
Library to obfuscate and deobfuscate 64bit integers (e.g. database key IDs) to uniform random looking 11 digit strings.

- Random unique string, **without** collisions! (Not really a real hash function because the input is fixed to 64 bit)
- Library implementation in C, native foreign language interface wrapper modules available for Ruby and PHP. (Not for speed reasons but to guarantee equality between implementations.) 
- Short! Perfect to use in URLs etc. `https://some.webservice.zzz/user/Gi7EPMODnHr`
- Reversible as fast as creation. (as said, not a real hash, only looks like one)

```
inasehash(42) -> "Gi7EPMODnHr"
inaseunhash("Gi7EPMODnHr") -> 42
```

# Why
- Prevent database IDs from going public, reducing the attack surface of your database infrastructure
- Telling your customer he/she is customer 79 does not feel right :)
- URL friendly characters



# Contains
- Command line tool
- Static library
- Ruby native foreign language interface wrapper
- PHP native foreign language interface wrapper
- Unit tests

# Usage Example


### As a Ruby module 
```ruby
require "inasehash"

hash = Inasehash::hash(42)

if Inasehash::isvalid?(hash)
    puts "Inasehash for '42' : " + hash
end

puts "Unhash of 'Gi7EPMODnHr' : " + Inasehash::unhash("Gi7EPMODnHr").to_s
```

### As C library 
```c
#include <inttypes.h>
#include <inasehash.h>

int main(int argc, char** argv) 
{
    char buf[HASH_LENGTH+1];

    inasehash(42, buf);

    printf("Inasehash of 42 is: %s\n", buf);

    char *hash = "Gi7EPMODnHr";

    if(is_valid_hash_string(hash)) {
        uint64_t rev = inaseunhash(hash);
        printf("Inasehash reverse of Gi7EPMODnHr is: %lld\n", rev);
    }

    return 0;
}
```

### From command line
```bash
$ inasehash 42
Gi7EPMODnHr
$ inasehash -d Gi7EPMODnHr
42
```

# Install

### Ruby Module
```bash
$ cd ruby-extension
$ ./make.sh
```
Now you can place the `inasehash.so` or respectively `inasehash.bundle` file in your ruby modules folder.

### PHP Module
```bash
$ cd php-extension
$ ./make.sh
```
Now you should add the `inasehash.so` file to the extension list in your php.ini file.



