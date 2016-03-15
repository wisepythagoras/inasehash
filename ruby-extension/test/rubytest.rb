#!/usr/bin/env ruby

require "../inasehash"

hash = Inasehash::hash(42)


if Inasehash::isvalid?(hash)
    puts "Inasehash for '42' : " + hash
end

puts "Unhash of 'Gi7EPMODnHr' : " + Inasehash::unhash("Gi7EPMODnHr").to_s

