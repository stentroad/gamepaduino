import sys
import os
import shlex
import subprocess

read_the_docs_build = os.environ.get('READTHEDOCS', None) == 'True'

if read_the_docs_build:
    subprocess.call('doxygen', shell=True)

extensions = ['breathe']
breathe_projects = { 'gamepad': 'xml' }
breathe_default_project = "gamepad"
templates_path = ['_templates']
source_suffix = '.rst'
master_doc = 'index'
project = u'gamepaduino'
copyright = u'(LGPL)+(cc-by-sa) marcos assis 2018'
author = u'marcos assis @metamero'
version = '1.0'
release = '1.0'
language = None
exclude_patterns = ['_build']
pygments_style = 'sphinx'
todo_include_todos = False
html_static_path = ['_static']
htmlhelp_basename = 'gamepadoc'
latex_elements = {
}
latex_documents = [
  (master_doc, 'gamepad.tex', u'gamepad Documentation',
   u'gamepad', 'manual'),
]
