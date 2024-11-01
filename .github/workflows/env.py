class env:
    @staticmethod
    def __getattr__( self, key ) -> str:
        from os import getenv;
        return str( getenv( key ) ) if getenv( key ) in self else '';
