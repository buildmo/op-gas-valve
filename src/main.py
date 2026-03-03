import socket
import yaml
from pathlib import Path


def load_config():
    hostname = socket.gethostname()
    config_path = Path(__file__).parent.parent / "configs" / f"{hostname}.yaml"

    if not config_path.exists():
        raise FileNotFoundError(
            f"No config found for '{hostname}'. Expected: {config_path}"
        )

    with open(config_path) as f:
        return yaml.safe_load(f)


if __name__ == "__main__":
    config = load_config()
    device = config["device"]
    print(f"Running on {device['name']} (role: {device['role']})")
