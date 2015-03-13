#!/usr/bin/python

def parse_header(path):
  enums = []
  functions = []
  ignored = set([
    'GL_FALSE',
    'GL_TRUE',
    'GL_VERSION_ES_CM_1_0',
    'GL_VERSION_ES_CL_1_0',
    'GL_VERSION_ES_CM_1_1',
    'GL_VERSION_ES_CL_1_1',
    'GL_ES_VERSION_2_0'])

  def parse_enum(line):
    tokens = line.split()
    if len(tokens) < 3:
      return
    define, name, value = tokens[:3]
    assert(define == '#define')
    if name in ignored:
      return
    enums.append((name, value))
  
  def parse_signature(sig):
    assert(sig[0] == '(')
    assert(sig[-2:] == ');')
    sig = sig[1:-2]
    if sig == 'void':
      return []
    arguments = []
    for type_name in sig.split(', '):
      _type, name = type_name.rsplit(None, 1)
      arguments.append((_type, name))
    return arguments
  
  def parse_function(line):
    # Strip whitespaces
    l, r = line.split('GL_APIENTRY')
    gl_apicall, _return = l.split(None, 1)
    name, signature = r.split(None, 1)
    assert(gl_apicall == 'GL_API' or gl_apicall == 'GL_APICALL')
    functions.append((name, parse_signature(signature), _return))

  for line in open(path):
    line = line.strip()
    if line.startswith('#define'):
      parse_enum(line)
    elif line.startswith('GL_APICALL ') or line.startswith('GL_API '):
      parse_function(line)

  return enums, functions

print parse_header('gl.h')

