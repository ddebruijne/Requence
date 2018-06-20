# Requence
Requence is a work-in-progress Unreal Engine 4 plugin that takes care of all functionality required to load and apply custom key bindings. Originally developed for Impeller Studio's Starfighter Inc's requirement of extremely customizable input - this can be used in any UE4 project to remove the development hassle that is input. Instead only focus on the UI, and let Requence do the heavy lifting!

This plugin is free to use and bundle in any (commercial) project! If your game starts to make big bucks however, a [donation](https://www.paypal.me/DannydeBruijne) is kindly appreciated!

## Current Features
- Rebinding, deleteing and adding both Axis and Action key bindings.
- Device-seperated input mappings, supporting multiple bindings for a branch
- Import & Export device presets with JSON (partially)
- Reset to default
- Unique device support - Mainly Joysticks - Using SDL to detect devices like the Thrustmaster T.16000
- Curve Editor

## Planned
- Steam Workshop Support
- Sample UI elements
- Documentation

## Thanks / References / Software Included
- Simple DirectMedia Layer: https://www.libsdl.org/
- JoystickPlugin: https://github.com/Ikarus76/UEJoystickPlugin/

## License
This plugin is free to use and bundle in any (commercial included) project. Re-releasing the plugin by itself however is not permitted. Source code must always remain open-source.

This plugin, or derivatives of this plugin are not allowed to be re-sold on any marketplace or storefront. This decision was made because the Unreal Marketplace does not allow selling of software that makes use of dependencies that are of the following license: GPL, LGPL, EPL, or MSPL, which SDL, a core part of this plugin, is. The [author of this plugin](https://github.com/danskidb) reserves the right to release a stripped down version of this software without SDL requirements. 

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
