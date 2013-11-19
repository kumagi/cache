out = 'build'

subdirs = ['src']

def options(opt):
  opt.load('compiler_cxx unittest_gtest boost')
  opt.recurse(subdirs)

def configure(conf):
  conf.env.CXXFLAGS += ['-Og', '-g', '-pipe', '-pthread',
                        '-Wall', '-Wextra', '-Wno-error=maybe-uninitialized']

  conf.load('compiler_cxx unittest_gtest')

  conf.env.LIB += ['pthread']
  conf.check_cxx(lib='pthread')
  conf.check_boost(lib='system thread')

  conf.recurse(subdirs)

def build(bld):
  bld.recurse(subdirs)
