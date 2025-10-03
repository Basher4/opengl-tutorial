project('cherno-opengl', 'cpp',
        version : '1.0.0',
        default_options : ['warning_level=3', 'cpp_std=c++23'])

glfw_dep = dependency('glfw3', 
                      fallback : ['glfw', 'glfw_dep'],
                      default_options : ['default_library=static'])
glew_dep = dependency('glew', default_options: ['default_library=static', 'warning_level=0'])
opengl_dep = dependency('gl')

cherno_opengl = executable('cherno_opengl', 'main.cpp',
                          dependencies : [glfw_dep, opengl_dep, glew_dep],
                          install : true)

test('test', cherno_opengl)