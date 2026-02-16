## Usage
```bash
# Deploy to all hosts
ansible-playbook -i inventory.ini deploy.yml

# Deploy to a single host
ansible-playbook -i inventory.ini deploy.yml --limit bulbasaur
```
