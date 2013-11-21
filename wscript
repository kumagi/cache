out = 'build'

subdirs = ['src']

def options(opt):
  opt.load('compiler_cxx unittest_gtest')
  opt.recurse(subdirs)

def configure(conf):
  conf.env.CXXFLAGS += ['-Og', '-ggdb', '-pipe', '-pthread',
                        '-Wall', '-Wextra', '-Wno-error=maybe-uninitialized']

  conf.load('compiler_cxx unittest_gtest')
  conf.env.LIB += ['pthread', 'event']

  conf.recurse(subdirs)

def build(bld):
  bld.recurse(subdirs)
