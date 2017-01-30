/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package parsewiki;

/**
 *
 * @author hiroshi
 */
import java.net.URL;
import java.io.File;
import java.io.IOException;

import org.jsoup.Jsoup;
import org.jsoup.nodes.Document;
import org.jsoup.parser.Parser;
import org.jsoup.select.Elements;
import org.jsoup.nodes.Element;

public class ParseWiki {

    /**
     * @param args the command line arguments
     */
    public static void main(String[] args) throws Exception
    {
        // TODO code application logic here
//        parseInfoBox();
        //test();
        parsing();
    }
    
    public static void test() throws IOException
    {
//        File input = new File("/Users/hiroshi/Desktop/a.html");
//        Document doc = Jsoup.parse(input, "UTF-8", "http://example.com/");
        Document doc = Jsoup.connect("http://en.wikipedia.org/wiki/Boston").get();
        Element link = doc.select("a").first();

        String text = doc.body().text(); // "An example link"
        String linkHref = link.attr("href"); // "http://example.com/"
        String linkText = link.text(); // "example""

        String linkOuterH = link.outerHtml(); 
        // "<a href="http://example.com"><b>example</b></a>"
        String linkInnerH = link.html(); // "<b>example</b>"
    }
    public static void parsing() throws Exception 
    {
        Document doc = Jsoup.connect("http://en.wikipedia.org/wiki/boston").get();
        Element contentDiv = doc.select("div[id=mw-content-text] > p").first();
        String paragraph = contentDiv.text();
        System.out.println(paragraph); // The result
    }
    
    public static void parseInfoBox() throws Exception
    {
        Document doc2 = Jsoup.connect("http://en.wikipedia.org/wiki/Tom_Cruise").get();
        Element body = doc2.body();
        Elements tables = body.getElementsByTag("table");
        for (Element table : tables) 
        {
            if (table.className().contains("infobox")==true) {
                System.out.println(table.outerHtml());
                table.outerHtml();
                break;
            }
        }        
    }
    
}
