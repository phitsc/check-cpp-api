from subprocess import check_output, check_call

def contains(col, item):
    return len(list(filter(lambda x: x.find(item) >= 0, col))) >= 1

def docker_image_exists(name):
    output = check_output([ 'docker', 'images' ]).decode("utf-8")
    return contains(output.splitlines(), name)

def docker_container_exists(name):
    output = check_output([ 'docker', 'ps', '-a' ]).decode("utf-8")
    return contains(output.splitlines(), name)
