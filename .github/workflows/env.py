class CEnv:
    def __init__( self ):
        super().__init__();
    def __getattr__( self, key ) -> str:
        from os import getenv;
        return str( getenv( key ) ) if getenv( key ) else '';
env: CEnv = CEnv();
