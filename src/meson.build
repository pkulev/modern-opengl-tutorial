sources = files([
  'index_buffer.cpp',
  'main.cpp',
  'vertex_buffer.cpp',
])

dependencies = [
  dependency('argparse'),
  dependency('glfw3'),
  dependency('glew'),
  dependency('gl'),
]

if host_machine.system() == 'darwin'
  add_project_link_arguments('-framework', 'OpenGL', language: 'cpp')
endif
