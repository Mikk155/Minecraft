from github import Github, GithubException
from env import env

try:

    g = Github( env.token );

    repo = g.get_repo( '{}/{}'.format( env.user, env.repository ) );

    release = repo.create_git_release( env.version, "# {}: {} ({})".format( env.game, env.mod, env.version ), '', draft=False, prerelease=False  );

    print( 'Generated release version {}'.format( env.version ) );

    print( 'Starting compiling binaries...');

except GithubException as e:

    if e.status == 422:

        print( 'WARNING! version {} Already exists. Update the enviroment variable VERSION in the github workflow file.'.format( env.version ) );

    print(e);

    exit(1);
