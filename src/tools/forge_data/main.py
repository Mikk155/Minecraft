import os

def printf( string: str | dict, arguments: dict | list[str] = [], cut_not_matched : bool = False, not_matched_trim : bool = False, dont_print : bool = False ) -> str:
    if isinstance( arguments, list ):
        for __arg__ in arguments:
            string = string.replace( "{}", str( __arg__ ), 1 ); # type: ignore
        if cut_not_matched:
            __replace__ = '{} ' if not_matched_trim else '{}';
            string.replace( __replace__, '' ); # type: ignore
    elif isinstance( arguments, dict ):
        for __oarg__, __narg__ in arguments.items():
            string = string.replace( "{"+__oarg__+"}", str( __narg__ ) ); # type: ignore
    else:
        raise Exception( __except__);
    if not dont_print:
        print( string );
    return string; # type: ignore

__LOGGER_LEVEL__ = 0

class LogLevel:
    '''Logger Level enums'''
    warning = ( 1 << 0 )
    '''Warning messages'''
    info = ( 1 << 1 )
    '''Informative messages'''
    debug = ( 1 << 2 )
    '''Debugging messages'''
    error = ( 1 << 3 )
    '''Error messages'''

class Logger:
    '''Logger instance'''

    @staticmethod
    def set_logger( logger_level: int, clear_level: bool = False ):
        '''
        Set a logger level

        ``clear_level`` if **True** instead of setting it will clear the ``LogLevel``
        '''
        global __LOGGER_LEVEL__;
        if not clear_level:
            if ( __LOGGER_LEVEL__ & logger_level ) == 0:
                __LOGGER_LEVEL__ |= logger_level;
        elif ( __LOGGER_LEVEL__ & logger_level ) != 0:
                __LOGGER_LEVEL__ &= ~logger_level;

    @staticmethod
    def __log__( log_type, message, arguments, logger_level ):
        global __LOGGER_LEVEL__;
        if ( __LOGGER_LEVEL__ & logger_level ) != 0:
            log_message = printf( message, arguments, True, True, True );
            print( '[{}] {}'.format( log_type, log_message ) );

    @staticmethod
    def debug( message: str | dict, arguments: dict | list[str] = [] ):
        '''Uses ``printf`` formatting'''
        Logger.__log__( 'Debug', message, arguments, LogLevel.debug );

    @staticmethod
    def info( message: str | dict, arguments: dict | list[str] = [] ):
        '''Uses ``printf`` formatting'''
        Logger.__log__( 'Info', message, arguments, LogLevel.info );

    @staticmethod
    def warning( message: str | dict, arguments: dict | list[str] = [] ):
        '''Uses ``printf`` formatting'''
        Logger.__log__( 'Warning', message, arguments, LogLevel.warning );

    @staticmethod
    def error( message: str | dict, arguments: dict | list[str] = [] ):
        '''Uses ``printf`` formatting'''
        Logger.__log__( 'Error', message, arguments, LogLevel.error );

def jsonc( obj : list[str] | str ) -> dict | list:
    '''
    Loads a text file and skips single-line commentary before loading a json object

    ``obj`` could be a path to a .json file or either an already opened file
    '''

    __js_split__ = '';
    __lines__: list[str];

    if isinstance( obj, list ):
        __lines__ = obj;
    else:
        __lines__ = open( obj, 'r' ).readlines();

    for __line__ in __lines__:

        __line__ = __line__.strip();

        if __line__ and __line__ != '' and not __line__.startswith( '//' ):
            __js_split__ = f'{__js_split__}\n{__line__}';

    from json import loads;
    return loads( __js_split__ )



Logger.set_logger( LogLevel.info );
Logger.set_logger( LogLevel.debug );
Logger.set_logger( LogLevel.error );
Logger.set_logger( LogLevel.warning );




class path:

    @staticmethod
    def __fix_slash__( string : str ) -> str:
        return string.replace( '\\', '/' );

    @staticmethod
    def __fix_backwards__( string : str ) -> str:
        while string.find( '/..' ) != -1:
            __find__ = string.find( '/..' );
            __sub1__ = string[ : __find__ ];
            __sub2__ = __sub1__[ : __sub1__.rfind( '/' ) + 1 ];
            __sub3__ = string[ __find__ + 4 : ];
            string = '{}{}'.format( __sub2__, __sub3__ );
        return string;

    @staticmethod
    def __fix_slash_backwards__( string : str ) -> str:
        return string.replace( '/', '\\' );

    @staticmethod
    def repo( Path: str ) -> str:
        return path.__fix_slash_backwards__( path.__fix_backwards__( path.__fix_slash__( '{}/../../../{}'.format( os.path.abspath( "" ), Path ) ) ) );

    @staticmethod
    def here( Path: str ) -> str:
        return path.__fix_slash_backwards__( path.__fix_slash__( '{}/{}'.format( os.path.abspath( "" ), Path ) ) );




global entitydata;
entitydata = {};

global FGD;
FGD = None;

global Hammer;
Hammer = None;




def write_keyvalues( entdata, classname ):

    global FGD;

    for key, data in entdata.items():

        variable = data.get( "variable", "" );

        value = data.get( "value", "" );

        if key == 'spawnflags':

            FGD.write( f'\tspawnflags(flags) =\n\t[\n' );

            sfbits = data.get( "choices", {} );

            for bits, bdata in sfbits.items():

                FGD.write( f'\t\t{bits} : "{bdata.get( "title", "" )}" : 0 : "{bdata.get( "description", "" )}"\n');

            FGD.write( f'\t]\n' );

        else:

            global Hammer;

            if Hammer:

                variables = {
                    "sky": "string",
                    "float": "string",
                    "scale": "string",
                    "vector": "string",
                    "target_name_or_class": "target_destination",
                    "target_generic": "target_destination"
                };

                if variable in variables:

                    variable = variables[ variable ];

            FGD.write( f'\t{key}({variable}) : "{data.get( "title", "" )}" : ' );

            if variable != 'choices':

                number = ( variable in [ "integer", "float" ] );

                if not number:

                    FGD.write( '"' );

                if number:

                    if not value or value == '':

                        if variable == 'float':

                            FGD.write( '0.0' );

                        else:

                            FGD.write( '0' );
                    else:

                        FGD.write( f'{value}' );
                else:

                    FGD.write( f'{value}' );

                if not number:

                    FGD.write( '"' );

                FGD.write( f' : "{data.get( "description", "" )}"\n');

            else:

                FGD.write( f'"{value}" : "{data.get( "description", "" )}" =\n\t[\n' );

                choices = data.get( "choices", {} );

                for choice, bchoice in choices.items():

                    FGD.write( f'\t\t"{choice}" : "{bchoice.get( "title", "" )}" : "{bchoice.get( "description", "" )}"\n');

                FGD.write( f'\t]\n' );




def write_class( entdata={}, name='' ):

    global FGD;

    Class = entdata.get( "Class", '' );

    FGD.write( f'@{Class}Class ' );

    for jkey, jvalue in entdata.items():

        if jkey == 'data':

            continue;

        elif jkey == 'size':

            min = str(jvalue[0]).replace( ',', '' ).strip( '[' ).strip( ']' );

            FGD.write( f'size( {min}' );

            if len(jvalue) > 1:

                max = str(jvalue[1]).replace( ',', '' ).strip( '[' ).strip( ']' );

                FGD.write( f', {max}' );

            FGD.write( f' ) ' );

        elif jkey == 'base' and len(jvalue) > 0:

            baseclasses = str(jvalue).replace( '\'', '' ).strip( '[' ).strip( ']' );

            FGD.write( f'base( {baseclasses} ) ' );

        elif jkey == 'color':

            colors = str(jvalue).replace( ',', '' ).strip( '[' ).strip( ']' );

            FGD.write( f'color( {colors} ) ' );

        elif jkey == 'studio':

            if isinstance( jvalue, str ):

                FGD.write( f'studio( "{jvalue}" ) ' );

            else:

                FGD.write( f'studio() ' );

        elif jkey == 'sprite':

            if isinstance( jvalue, str ):

                FGD.write( f'sprite( "{jvalue}" ) ' );

            else:

                FGD.write( f'sprite() ' );

        elif jkey == 'flags':

            FGD.write( f'flags( {jvalue} ) ' );

        elif jkey == 'iconsprite':

            FGD.write( f'iconsprite( "{jvalue}" ) ' );

        elif jkey == 'offset':

            offset = str(jvalue).replace( ',', '' ).strip( '[' ).strip( ']' );

            FGD.write( f'offset( {offset} ) ' );

    FGD.write( f'= {name}' );

    if Class == 'Base':

        FGD.write( '\n[\n' );

    else:

        FGD.write( f' : "{entdata.get( "title", "" )}" : "{entdata.get( "description", "" )}"\n[\n' );

    if "data" in entdata:

        write_keyvalues( entdata.get( "data", {} ), name );

    FGD.write( ']\n\n' );

    if entdata.get( 'point', False ):

        entdata.pop( 'point', '' );

        entdata.pop( 'data', '' );

        entdata[ 'Class' ] = 'Point';

        AddHulls = entdata.get( 'base', [] ).copy();

        if not 'hulls' in AddHulls:

            for b in [ 'Angles', 'Targetx', 'Target', 'Master', 'Global', 'Mandatory' ]:


                if b in AddHulls:

                    AddHulls.insert( AddHulls.index( b ) + 1, f'hulls' );

                    break;

            entdata[ 'base' ] = AddHulls;

        write_class( entdata=entdata, name=name );




def write_data( key, value={} ):

    if value.get( 'Class', '' ) in [ 'Point', 'Solid' ] and key != 'worldspawn':

        if len( value.get( 'data', {} ) ) > 0:

            KeyValueDatas = value.get( 'data', {} );

            value.pop( 'data', '' );

            value[ 'own_data' ] = KeyValueDatas;

            write_class( entdata={ "Class": "Base", "data": KeyValueDatas }, name=f'EntClass{key}' );

            base = value.get( 'base', [] );

            if not f'EntClass{key}' in base:

                for b in [ 'Angles', 'Targetx', 'Target', 'Master', 'Global', 'Mandatory' ]:

                    if b in base:

                        base.insert( base.index( b ) + 1, f'EntClass{key}' );

                        break;

                if not f'EntClass{key}' in base:

                    base.insert( 0, f'EntClass{key}');

                value[ 'base' ] = base;

        # So not to include them on each entity manually. since all entities supports these
        Mandatory = value.get( 'base', [] );

        if not 'Appearflags' in Mandatory:

            Mandatory.append( 'Appearflags' );

        if not 'Mandatory' in Mandatory:

            Mandatory.insert( 0, 'Mandatory' );

        value[ 'base' ] = Mandatory;

    write_class( entdata=value, name=key );




def build():

    def load_json_files( folder: str ):

        global entitydata;

        for file in os.listdir( path.here( folder ) ):

            try:

                entitydata[ file[ : len(file) - len('.json') ] ] = jsonc( path.here( '{}/{}'.format( folder, file ) ) );

                Logger.debug( "Loaded {}", [ file ] );

            except Exception as e:

                Logger.warning( "Failed to open \"{}\" Exception: {}", [ file, e ] );

        Logger.info( "Finished loading json files from {}.", [ folder ] );

    Logger.info( "Loading configuration files..." );

    load_json_files( "base" );

    load_json_files( "entities" );

    entities = []

    classess = []

    global entitydata;

    for ClassName, ClassData in entitydata.items():

        if ClassData.get( 'Class', '' ) in [ 'Point', 'Solid' ]:

            entities.append( ClassName );

        else:

            classess.append( ClassName );

    Logger.info( "Sorting classnames..." );

    entities.sort();

    programs = [ 'Hammer', 'JACK' ];

    for program in programs:

        Logger.info( "Generating FGD for {}...", [ program ] );

        global Hammer;

        Hammer = ( program == 'Hammer' );

        if not os.path.exists( path.repo( "game/fgd/" ) ):

            os.makedirs( path.repo( "game/fgd/" ) );

        global FGD;

        FGD = open( path.repo( "game/fgd/minecraft-{}.fgd".format( program ) ), 'w' );

        FGD.write( '''
//============ Copyright 1996-2005, Valve Corporation, All rights reserved. ======================//
//                                                                                                //
// Purpose: Half-Life: Minecraft game definition file (.fgd)                                      //
//                                                                                                //
// This file has been generated by a python script                                                //
// https://github.com/Mikk155/Minecraft/blob/master/src/forge_data/main.py                        //
//                                                                                                //
// DO NOT MODIFY THIS FILE, SEE https://github.com/Mikk155/Minecraft/tree/master/src/forge_data   //
//                                                                                                //
//================================================================================================//

''' );

        for key in classess:

            write_data( key, entitydata.get( key, {} ).copy() )

        for key in entities:

            write_data( key, entitydata.get( key, {} ).copy() )




build();
