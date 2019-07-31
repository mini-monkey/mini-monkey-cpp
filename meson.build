project('minimonkey-c', 'cpp',
	Default_options : ['cpp_std=c++17'])

add_project_arguments(['-DBOOST_LOG_DYN_LINK'],
  language : 'cpp'
)

# thread_dep = dependency('threads')
# boost_dep = dependency('boost', modules : ['thread'])
boost_dep = dependency('boost', modules : ['thread', 'utility', 'system'])

executable('mm_pub', 'publisher.cc', dependencies : boost_dep)
