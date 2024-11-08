from git import Repo
import os
import shutil
from env import env

repo_url = 'https://{}@github.com/{}/{}.wiki.git'.format( env.token, env.user, env.repository );

abs = os.path.abspath("");

clone_dir = '{}/../{}-wiki/'.format( abs, env.repository );

if not os.path.exists( clone_dir ):

    os.makedirs( clone_dir );

repo = Repo.clone_from( repo_url, clone_dir );

for file in os.listdir( clone_dir ):

    if not file.endswith( ".md" ):

        continue;

    if not os.path.exists( '{}/docs/{}'.format( abs, file ) ):

        os.remove( '{}/{}'.format( clone_dir, file ) );

shutil.copytree( "docs/", clone_dir, dirs_exist_ok=True );

repo.git.add( all=True );

repo.index.commit( "Update wiki" );

origin = repo.remote( name="origin" );

origin.push();
