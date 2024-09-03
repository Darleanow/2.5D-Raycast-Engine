import os
import sys
sys.path.insert(0, os.path.abspath('.'))

extensions = [
    'breathe',
]

breathe_projects = {
    "Raycaster": "../docs/xml/"
}

breathe_default_project = "Raycaster"

html_theme = 'furo'

templates_path = ['_templates']
source_suffix = ['.rst', '.md']
master_doc = 'index'

project = 'Raycaster'
author = 'Darleanow'
release = '1.0'

html_static_path = ['_static']
