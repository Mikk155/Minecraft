import os
import shutil
from git import Repo

user: str = os.getenv[ "REPOSITORY" ].split( '/' )[0];

repository: str = os.getenv[ "REPOSITORY" ].split( '/' )[1];

token: str = os.getenv[ "TOKEN" ];

repo_url = f'https://{token}@github.com/{user}/{repository}.wiki.git';

abs = os.path.abspath("");

clone_dir: str = f'{abs}/../{repository}-wiki/';

if not os.path.exists( clone_dir ):

    os.makedirs( clone_dir );

repo = Repo.clone_from( repo_url, clone_dir );

for file in os.listdir( clone_dir ):

    if not file.endswith( ".md" ):

        continue;

    if not os.path.exists( f'{abs}/docs/{file}' ):

        os.remove( f'{clone_dir}/{file}' );

shutil.copytree( "docs/", clone_dir, dirs_exist_ok=True );

repo.git.add( all=True );

repo.index.commit( "Update wiki" );

origin = repo.remote( name="origin" );

origin.push();
