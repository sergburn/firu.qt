/****************************************************************************
**
** Trolltech hereby grants a license to use the Qt/Eclipse Integration
** plug-in (the software contained herein), in binary form, solely for the
** purpose of creating code to be used with Trolltech's Qt software.
**
** Qt Designer is licensed under the terms of the GNU General Public
** License versions 2.0 and 3.0 ("GPL License"). Trolltech offers users the
** right to use certain no GPL licensed software under the terms of its GPL
** Exception version 1.2 (http://trolltech.com/products/qt/gplexception).
**
** THIS SOFTWARE IS PROVIDED BY TROLLTECH AND ITS CONTRIBUTORS (IF ANY) "AS
** IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED
** TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A
** PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER
** OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
** EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
** PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
** PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
** LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
** NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
** SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** Since we now have the GPL exception I think that the "special exception
** is no longer needed. The license text proposed above (other than the
** special exception portion of it) is the BSD license and we have added
** the BSD license as a permissible license under the exception.
**
****************************************************************************/

#include "firumainwindow.h"
#include "firuqt.h"
#include "trainerdialog.h"

#include <QtGui>
#include <QMessageBox>
#include "firuapp.h"

int main(int argc, char *argv[])
{
    FiruApp a(argc, argv);
#ifdef QT_KEYPAD_NAVIGATION
    a.setKeypadNavigationEnabled( false );
#endif

    qDebug() << "I'm at " << QDir::currentPath();
    bool isOpen = a.openDatabase();    
    qDebug() << "db open? " << isOpen; 
    
    int res = 0;
    if ( isOpen )
    {
        FiruMainWindow w;
        w.show();

//        Dictionary::Ptr d = Dictionary::Ptr( new Dictionary( LangPair( QLocale::Finnish, QLocale::Russian ) ) );
//        if ( d->open() )
//        {
//            FiruApp::startTrainer( d );
//        }
        
        QString importDict;
        QStringList args = a.arguments();
        for ( int i = 1; i < args.count(); i++ )
        {
            if ( args[i] == "-import" && args.count() > i + 1 )
            {
                importDict = args[i+1];
                i++;
            }
        }
        if ( importDict.length() )
        {
            w.importDict( importDict );
        }
        
        res = a.exec();
    }
    else
    {
        res = 1;
    }
    return res;
}
