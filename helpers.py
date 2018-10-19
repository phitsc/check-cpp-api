""" Some helper functions """

from subprocess import check_output


def contains(col, item):
    """ Returns true if the collection col contains the item item. """
    return len(list(filter(lambda x: x.find(item) >= 0, col))) >= 1


def docker_image_exists(name):
    """ Returns true if a Docker image named name exists. """
    output = check_output(["docker", "images"]).decode("utf-8")
    return contains(output.splitlines(), name)


def docker_container_exists(name):
    """ Returns true if a Docker container named name exists. """
    output = check_output(["docker", "ps", "-a"]).decode("utf-8")
    return contains(output.splitlines(), name)
