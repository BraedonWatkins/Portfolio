import discord
import random
import os
from discord.ext import commands
const { token } = require('./config.json')

intents = discord.Intents.default()
intents.members = True

client = commands.Bot(command_prefix='.', intents=intents)

@client.event
async def on_ready():
    print('Bot is ready.')

@client.event
async def on_member_join(member):
    print(f'{member} has joined the server!')

@client.event
async def on_member_remove(member):
    print(f'toodles {member}.')

@client.command()
async def load(ctx, extension):
    client.load_extension(f'cogs.{extension}')

@client.command()
async def unload(ctx, extension):
    client.unload_extension(f'cogs.{extension}')



@client.command()
async def kick(ctx, member : discord.member, *, reason = None):
    await member.kick(reason = reason)

@client.command()
async def ban(ctx, member : discord.member, *, reason = None):
    await member.ban(reason = reason)

@client.command()
async def unban(ctx, *, member):
    bannedUsers = await ctx.guild.bans()
    memberName, memberDiscriminator = member.split('#')

    for banEntry in bannedUsers:
        user = banEntry.user

        if ((user.name, user.discriminator) == (memberName, memberDiscriminator)):
            await ctx.guild.unban(user)
            await ctx.send(f'Unbanned {user.mention}')
            return

@client.command()
async def clear(ctx, amount = 2):
    await ctx.channel.purge(limit = amount)

@client.command(aliases = ["8ball", "eightball", "eight ball", "8 ball"])
async def _8ball(ctx, *, question):
    responses = ["get recked", "for sure", "ask me later I'm watchin the game"]
    await ctx.send(f'Question: {question}\nAnswer: {random.choice(responses)}')

@client.command()
@commands.is_owner()
async def shutdown(ctx):
    await ctx.send("Goodbye (^)>")
    #await ctx.channel.purge(3)
    await ctx.bot.logout()



for filename in os.listdir('./cogs'):
    if filename.endswith('.py'):
        client.load_extension(f'cogs.{filename[:-3]}')

client.login(token)
