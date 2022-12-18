project(
  'modern-opengl-tutorial', 'cpp',
  version: '1.0.0',
  default_options: [
    'default_library=static',
    'cpp_std=c++17',
  ],
  meson_version: '>=0.64.1',
)

# Provides 'sources' and 'dependencies' variables.
subdir('src')

executable('demo', sources, dependencies: dependencies)
