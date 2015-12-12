import os
import sys
joinp = os.path.join
sys.path.insert(0, 'whitgl')
sys.path.insert(0, joinp('whitgl', 'input'))
import build
import platform
import ninja_syntax

def main():
  target = 'space'
  srcdir = 'src'
  inputdir = joinp('whitgl', 'input')
  builddir = 'build'
  targetdir = joinp(builddir, 'out')
  objdir = joinp(builddir, 'obj')
  libdir = joinp(builddir, 'lib')
  data_in =  'data'
  data_out = joinp(targetdir, 'data')
  buildfile = open('build.ninja', 'w')
  n = ninja_syntax.Writer(buildfile)
  cflags = build.cflags + ' -Iwhitgl/inc -Isrc'
  build.rules(n, cflags, build.ldflags)
  obj = build.walk_src(n, srcdir, objdir)
  whitgl = [joinp('whitgl','build','lib','whitgl.a')]
  targets = []
  targets += n.build(joinp(targetdir, target), 'link', obj+whitgl)
  n.newline()

  data = build.walk_data(n, data_in, data_out, ['png','ogg','lvl'])

  targets += n.build('data', 'phony', data)
  n.newline()

  targets += build.copy_libs(n, inputdir, targetdir)

  n.build('all', 'phony', targets)
  n.default('all')

if __name__ == '__main__':
  main()
