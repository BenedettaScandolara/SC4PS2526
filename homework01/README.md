# HomeWork 01 — Setting up a Linux machine on CloudVeneto for C codes

This guide describes how to create a virtual machine (VM) on CloudVeneto, connect to it from a **Mac** (macOS terminal), install the tools needed to compile C codes, and compile and run a first program.

---

## 1. Get a CloudVeneto account

1. Go to the CloudVeneto dashboard, <https://cloudveneto.ict.unipd.it/dashboard>, and log in with the UniPD (SSO) credentials.
2. Fill in the registration form and ask to join the course project, **`SC4PS-PhD`**.
3. When the request is approved, you receive an e-mail with the username and password for the **gate machine** `gate.cloudveneto.it`. The VMs are not reachable directly from the internet: every connection passes through this gate.

Test the access to the gate from the Mac terminal (*Applications → Utilities → Terminal*):

```bash
ssh USERNAME@gate.cloudveneto.it
```

At the first login you are asked to change the password. Then close the connection with `exit`.

## 2. Create an SSH key pair

The key pair is used to log into the VM without a password.

1. In the dashboard, select the project `SC4PS-PhD` and go to **Compute → Key Pairs → Create Key Pair**.
2. Give it a name (e.g. `sc4ps_key`), choose type **SSH Key** and create it. The file `sc4ps_key.pem` (the private key) is downloaded.
3. Move it into the `~/.ssh` folder and make it readable only by you (otherwise `ssh` refuses to use it):

```bash
mkdir -p ~/.ssh
mv ~/Downloads/sc4ps_key.pem ~/.ssh/
chmod 600 ~/.ssh/sc4ps_key.pem
```

## 3. Create the virtual machine

In the dashboard go to **Compute → Instances → Launch Instance** and set:

| Tab | Setting |
|---|---|
| **Details** | Instance name, e.g. `sc4ps-vm` |
| **Source** | Boot source *Image*, e.g. an **AlmaLinux** image (`AlmaLinux10-...`), no new volume |
| **Flavor** | e.g. `cldareapd.large` (8 GB RAM, useful for large vectors and matrices) |
| **Key Pair** | the key created in step 2 |

Click **Launch Instance**. After a few seconds the VM is *Running* and the dashboard shows its **IP address** (something like `10.67.x.x`): write it down.

## 4. Connect to the VM

The connection goes through the gate with the option `-J` (*jump host*):

```bash
ssh -J USERNAME@gate.cloudveneto.it -i ~/.ssh/sc4ps_key.pem almalinux@IP_ADDRESS
```

- `USERNAME` is the CloudVeneto username (for the gate);
- `almalinux` is the default user of the AlmaLinux images;
- `IP_ADDRESS` is the IP of the VM.

### Shorter command with `~/.ssh/config`

To avoid writing the full command each time, add these lines to the file `~/.ssh/config` on the Mac (create it with `nano ~/.ssh/config` if it does not exist):

```
Host cloudveneto
    HostName gate.cloudveneto.it
    User USERNAME

Host sc4ps
    HostName IP_ADDRESS
    User almalinux
    IdentityFile ~/.ssh/sc4ps_key.pem
    ProxyJump cloudveneto
    ServerAliveInterval 60
```

Now it is enough to type:

```bash
ssh sc4ps
```

(`ServerAliveInterval 60` keeps the connection alive during long runs.)

To copy files between the Mac and the VM, `scp` uses the same configuration:

```bash
scp myfile.c sc4ps:~/          # Mac -> VM
scp sc4ps:~/results.txt .      # VM  -> Mac
```

## 5. Install the tools for C on the VM

AlmaLinux uses the package manager `dnf`; installing software needs `sudo`.

```bash
# update the system
sudo dnf upgrade -y

# compiler gcc, make, gdb, git, ...
sudo dnf group install -y "Development Tools"

# extra repositories (needed for some -devel packages)
sudo dnf install -y epel-release
sudo dnf config-manager --set-enabled crb

# libraries and tools used in the next homeworks
sudo dnf install -y gsl gsl-devel nano python3 python3-pip
```

Check that everything works:

```bash
gcc --version
make --version
git --version
```

## 6. Write, compile and run a first C program

Create the file with a terminal editor (`nano hello.c`; in nano save with `Ctrl+O`, `Enter` and exit with `Ctrl+X`):

```c
#include <stdio.h>

int main(void)
{
    printf("Hello World from my CloudVeneto VM!\n");
    return 0;
}
```

Compile and run:

```bash
gcc -Wall -O2 hello.c -o hello
./hello
```

Output:

```
Hello World from my CloudVeneto VM!
```

Useful `gcc` options:

| Option | Meaning |
|---|---|
| `-o name` | name of the executable (default `a.out`) |
| `-Wall` | show warnings about possible errors |
| `-O2` | optimisation |
| `-lm` | link the math library (`sqrt`, `sin`, `exp`, ...) |
| `-g` | debug information (for `gdb`) |

## 7. Exercise for the lecture of Tim Mattson: `learningC.c`

Download and run the file `learningC.c` from the repository [tgmattso/CompSciForPhys](https://github.com/tgmattso/CompSciForPhys):

```bash
git clone https://github.com/tgmattso/CompSciForPhys.git
cd CompSciForPhys
gcc learningC.c -o learningC
./learningC
```

The file is a short introduction to C written as a commented program: variables and types (`int`, `float`, `double`), `printf`, loops, scope of variables, arrays, pointers and functions. Running it prints, for example:

```
 my int is 5 and my float is 3.400000, and my double is 7.800000
 ...
 sum before pointFunc = 10.000000
 sum computed with array notation = 10
 sum computed with pointer notation = 10
 sum after pointFunc = 20.000000
```

## 8. (Optional) Use git from the VM

To work on this repository directly from the VM:

```bash
git config --global user.name  "Benedetta Scandolara"
git config --global user.email "benedetta.scandolara@phd.unipd.it"

# create a key for GitHub and copy the public key in
# GitHub -> Settings -> SSH and GPG keys -> New SSH key
ssh-keygen -t ed25519 -C "benedetta.scandolara@phd.unipd.it"
cat ~/.ssh/id_ed25519.pub

git clone git@github.com:USERNAME_GITHUB/SC4PS.git
cd SC4PS
# ... modify files ...
git add .
git commit -m "Add HomeWork 02"
git push
```

## Checklist

- [ ] CloudVeneto account and access to `gate.cloudveneto.it`
- [ ] SSH key pair created and saved in `~/.ssh` with `chmod 600`
- [ ] VM created with an AlmaLinux image and the key pair
- [ ] Connection with `ssh sc4ps`
- [ ] `gcc`, `make`, `git`, `gsl` installed
- [ ] `hello.c` compiled and executed
- [ ] `learningC.c` compiled and executed
