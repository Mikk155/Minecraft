from git import Repo
import os
import shutil
from env import env

repo_url = 'https://{}@github.com/{}/{}.wiki.git'.format( env.token, env.user, env.repository );

clone_dir = '../{}-wiki/'.format( env.repository );

if not os.path.exists( clone_dir ):

    os.makedirs( clone_dir );

repo = Repo.clone_from( repo_url, clone_dir );

shutil.copytree( "docs/", clone_dir, dirs_exist_ok=True );

repo.git.add( all=True );

repo.index.commit( "Update wiki" );

origin = repo.remote( name="origin" );

origin.push();
