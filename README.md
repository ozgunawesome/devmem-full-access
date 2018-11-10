# devmem-full-access
Allow unrestricted access to `/dev/mem` device on restricted Linux kernels.

If the kernel is compiled with the option `CONFIG_STRICT_DEVMEM=y` the `/dev/mem` block device won't allow access beyond the first 1 MB of memory.

Learn more here: [https://lwn.net/Articles/267427/](https://lwn.net/Articles/267427/)

This kernel module can be used to bypass that restriction without having to recompile the kernel.

## Usage
* clone repository
* `make`
* `sudo insmod devmem-full-access.ko`

## Testing
* `sudo dd if=/dev/mem of=/dev/null bs=1 skip=1048576 count=1`
* If your kernel has strict devmem enabled, you'll see the message `dd: error reading ‘/dev/mem’: Operation not permitted`
* After loading the module you'll see `1 byte (1 B) copied` and this means you may read/write/seek to any point in memory.

## Removal
* `sudo rmmod devmem-full-access`

## License
GPL

## Warranty
Absolutely none. Your system may go up in flames at any given moment. 

###### THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.


