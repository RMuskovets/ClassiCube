﻿using System;
using System.IO;
using System.Net;
using System.Windows.Forms;

namespace Launcher {
	
	public class ResourceFetcher {
		
		const string resUri = "https://raw.githubusercontent.com/andrewphorn/ClassiCube-Client/master/src/main/resources/";
		
		public void Run( MainForm form ) {
			using( WebClient client = new WebClient() ) {
                client.Proxy = null;
				if( !DownloadData( "char.png", client, "char.png", form ) ) return;
				if( !DownloadData( "mob/chicken.png", client, "chicken.png", form ) ) return;
				if( !DownloadData( "clouds.png", client, "clouds.png", form ) ) return;
				if( !DownloadData( "mob/creeper.png", client, "creeper.png", form ) ) return;
				//if( !DownloadData( "mob/croc.png", client, "croc.png", form ) ) return;
				if( !DownloadData( "mob/pig.png", client, "pig.png", form ) ) return;
				//if( !DownloadData( "mob/printer.png", client, "printer.png", form ) ) return;
				if( !DownloadData( "mob/sheep.png", client, "sheep.png", form ) ) return;
				if( !DownloadData( "mob/sheep_fur.png", client, "sheep_fur.png", form ) ) return;
				if( !DownloadData( "mob/skeleton.png", client, "skeleton.png", form ) ) return;
				if( !DownloadData( "mob/spider.png", client, "spider.png", form ) ) return;
				if( !DownloadData( "terrain.png", client, "terrain.png", form ) ) return;
				if( !DownloadData( "mob/zombie.png", client, "zombie.png", form ) ) return;
			}
		}
		
		static bool DownloadData( string uri, WebClient client, string output, MainForm form ) {
			if( File.Exists( output ) ) return true;
			form.Text = MainForm.AppName + " - fetching " + output;
			
			byte[] data = null;
			try {
				data = client.DownloadData( resUri + uri );
			} catch( WebException ) {
				MessageBox.Show( "Unable to download " + output, "Failed to download resource", MessageBoxButtons.OK, MessageBoxIcon.Error );
				return false;
			}
			
			try {
				File.WriteAllBytes( output, data );
			} catch( IOException ) {
				MessageBox.Show( "Unable to save " + output, "Failed to save resource", MessageBoxButtons.OK, MessageBoxIcon.Error );
				return false;
			}
			return true;
		}

        public bool CheckAllResourcesExist() {
            return CheckResourcesExist("char.png", "chicken.png", "clouds.png", "creeper.png", "pig.png", "sheep.png",
                                       "sheep_fur.png", "skeleton.png", "spider.png", "terrain.png", "zombie.png");
        }

        static bool CheckResourcesExist(params string[] resources) {
            foreach (string file in resources) {
                if (!File.Exists(file))
                    return false;
            }
            return true;
        }
    }
}
