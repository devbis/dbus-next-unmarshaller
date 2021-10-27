from distutils.core import setup, Extension

setup(
    name="dbus_next_unmarshaller",
    version="1.0",
    ext_modules=[
          Extension("dbus_next_unmarshaller", [
                "src/bindings.c",
                "src/unmarshaller.c",
          ]),
    ],
)
