# Convert png's invisible pixels to a pure blue

This python script will convert the png's invisible pixels to a pure blue and rename the file with a leading ``{`` so you can simply import to wally and then choose the last palette color.

```
pip install Pillow
```

```py
from PIL import Image
import os

all_files = [];

# Folder name of where the textures are located at
folder_path = "block/";

for filename in os.listdir( folder_path ):

    file_path = os.path.join( folder_path, filename );

    modify = False;

    if os.path.isfile( file_path ) and filename.lower().endswith( '.png' ):

        img = Image.open( file_path );

        if img.mode == 'RGBA':

            img = img.convert( "RGBA" );

            data = img.getdata();

            new_data = [];

            for item in data:

                if item[3] == 0:

                    new_data.append( ( 0, 0, 255, 255 ) );

                    modify = True;

                else:

                    new_data.append( item );

            if modify:

                img.putdata( new_data );

                img.save( file_path, "PNG" );

                name, extension = os.path.splitext( filename );

                new_name = "{" + name + extension;

                new_file_path = os.path.join( folder_path, new_name );

                img.close();

                os.rename( file_path, new_file_path );

                all_files.append( new_file_path );

                print( 'Updated: {}'.format( new_name ) );
```