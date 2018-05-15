import sys
import cx_Freeze
import os.path
PYTHON_INSTALL_DIR = os.path.dirname(os.path.dirname(os.__file__))
os.environ['TCL_LIBRARY'] = os.path.join(PYTHON_INSTALL_DIR, 'tcl', 'tcl8.6')
os.environ['TK_LIBRARY'] = os.path.join(PYTHON_INSTALL_DIR, 'tcl', 'tk8.6')

executables = [cx_Freeze.Executable("MonochromatorGUI.py")]

cx_Freeze.setup(

    name="Monochromator",
    version='1.1',
    options={"build_exe": {"packages": [ "serial", "re", "kivy", "threading","time", "sys", "os","kivy.deps.angle", "kivy.deps.glew", "kivy.deps.gstreamer", "kivy.deps.sdl2",  "PIL", "OpenGL", "pyparsing", "kivy"]}},
    description="GUI for Monochromator",
    executables=executables
)
