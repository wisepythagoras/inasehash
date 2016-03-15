require 'mkmf'

extension_name = 'inasehash'

have_header(extension_name + ".h")

dir_config(extension_name)
create_makefile(extension_name)
